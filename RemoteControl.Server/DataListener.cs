using log4net;
using RemoteControl.Net.Enums;
using RemoteControl.Server.Data;
using System.Diagnostics;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace RemoteControl.Server
{
    public class DataListener : IDataListener
    {
        private const int AcceptTimeout = 2000;
        private const int StreamTimeout = 100;
        private const int BufferSize = 4096;

        private readonly TcpListener _tcpListener;

        private readonly ILog _logger;

        private readonly IRouter _router;
        private readonly IMap<string, EndPoint> _udpEndPoints;
        private readonly IMap<string, EndPoint> _tcpEndPoints;

        private IDictionary<EndPoint, Socket> _sockets;
        private IDictionary<EndPoint, Stream> _netStreams;

        private Thread _dataThread;
        private Mutex _streamMutex;


        public bool IsRunning { get; private set; }

        public int Port => (_tcpListener?.LocalEndpoint as IPEndPoint)?.Port ?? 0;


        public DataListener(
            IPAddress iPAddress,
            int port,
            IRouter router,
            IMap<string, EndPoint> udpEndPoints,
            IMap<string, EndPoint> tcpEndPoints,
            ILog logger
        )
        {
            _tcpListener = new TcpListener(iPAddress, port);
            _tcpListener.Server.ReceiveTimeout = AcceptTimeout;
            _tcpListener.Server.SendTimeout = AcceptTimeout;

            _router = router;
            _udpEndPoints = udpEndPoints;
            _tcpEndPoints = tcpEndPoints;

            _sockets = new Dictionary<EndPoint, Socket>();
            _netStreams = new Dictionary<EndPoint, Stream>();

            _logger = logger;
        }

        public void Dispose()
        {
            
        }

        public void Run() 
        {
            IsRunning = true;
            
            _streamMutex = new Mutex(false);
            _dataThread = new Thread(Listen);
            _dataThread.Start();

            _tcpListener.Start();
            _logger.Info($"data listener running at port {Port}");

            var tasks = new List<Task>();

            while (IsRunning) 
            {
                Socket client = null;

                try
                {
                    client = _tcpListener?.AcceptSocket();
                }
                catch (SocketException e) 
                {
                    // not WSATIMEDOUT
                    if (e.ErrorCode != 10060) 
                    {
                        throw e;
                    }
                }

                tasks.Add(ProcessAcceptedClientAsync(client));
            }

            _tcpListener.Stop();
        }

        public void Close()
        {
            
        }

        public void DisconnectInactive() 
        {
            _streamMutex.WaitOne();

            _sockets = _sockets.Where(p => p.Value.Connected).ToDictionary(p => p.Key, p => p.Value);

            _netStreams = _netStreams.Where(p => _sockets.ContainsKey(p.Key)).ToDictionary(p => p.Key, p => p.Value);

            var endPoints = new EndPoint[_tcpEndPoints.Forward.Keys.Count];
            _tcpEndPoints.Forward.Values.CopyTo(endPoints, 0);

            foreach (var endPoint in endPoints)
            {
                if (!_sockets.ContainsKey(endPoint)) 
                {
                    _tcpEndPoints.Reverse.Remove(endPoint);
                }
            }

            _streamMutex.ReleaseMutex();
        }

        private void Listen() 
        {
            var redirectionTasks = new Dictionary<EndPoint, Task>();

            while (IsRunning) 
            {
                if (_netStreams.Count < 2) 
                {
                    continue;
                }

                //_sockets = _sockets.Where(p => p.Value.Connected).ToDictionary(p => p.Key, p => p.Value);
                //_netStreams = _netStreams.Where(p => _sockets.ContainsKey(p.Key)).ToDictionary(p => p.Key, p => p.Value);
                //redirections = redirections.Where(p => _sockets.ContainsKey(p.Key)).ToDictionary(p => p.Key, p => p.Value);


                _streamMutex.WaitOne();

                foreach (var endPoint in _netStreams.Keys) 
                {
                    bool canRedirect = _tcpEndPoints.Reverse.ContainsKey(endPoint) && _router.Resolve(_tcpEndPoints.Reverse[endPoint]).Count > 0;

                    if (!redirectionTasks.ContainsKey(endPoint) && canRedirect)
                    {
                        redirectionTasks[endPoint] = Task.WhenAny(Redirect(endPoint), Task.Delay(StreamTimeout));
                    }
                }

                _streamMutex.ReleaseMutex();

                Task.WaitAny(
                    Task.WhenAny(redirectionTasks.Values),
                    Task.Delay(StreamTimeout)
                );

                foreach (var key in redirectionTasks.Keys.ToArray()) 
                {
                    bool canRedirect = _tcpEndPoints.Reverse.ContainsKey(key) && _router.Resolve(_tcpEndPoints.Reverse[key]).Count > 0;

                    if (redirectionTasks[key].IsCompleted || !canRedirect) 
                    {
                        redirectionTasks.Remove(key);
                    }
                }
            }
        }

        private async Task ProcessAcceptedClientAsync(Socket socket) 
        {
            var connectionStringBytes = new byte[8];
            await socket.ReceiveAsync(connectionStringBytes, SocketFlags.None);

            var connectionsString = Encoding.UTF8.GetString(connectionStringBytes);
            _logger.Info($"accepting {socket.RemoteEndPoint} with string \"{connectionsString}\"");

            if (!_udpEndPoints.Forward.ContainsKey(connectionsString)) 
            {
                _logger.Error($"bad connection string");
                await socket.SendAsync(new byte[] { (byte)NetResult.Error }, SocketFlags.None);
                return;
            }

            _tcpEndPoints.Forward[connectionsString] = socket.RemoteEndPoint;
            _sockets[socket.RemoteEndPoint] = socket;

            _streamMutex.WaitOne();
            _netStreams[socket.RemoteEndPoint] = new NetworkStream(socket);
            _streamMutex.ReleaseMutex();

            _logger.Info($"{socket.RemoteEndPoint} accepted");
            await socket.SendAsync(new byte[] { (byte)NetResult.Ok }, SocketFlags.None);
        }

        private async Task Redirect(EndPoint endPoint) 
        {
            var stream = _netStreams[endPoint];
            var connectionString = _tcpEndPoints.Reverse[endPoint];

            var buffer = new byte[BufferSize];
            int count = await stream.ReadAsync(buffer, 0, buffer.Length);

            var tasks = new List<Task>();

            foreach (var sendConnectionString in _router.Resolve(connectionString))
            {
                var sendEndPoint = _tcpEndPoints.Forward[sendConnectionString];
                var sendStream = _netStreams[sendEndPoint];

                //Debug.WriteLine($"Sending {count} bytes of data from {endPoint} to {sendEndPoint}");
                tasks.Add(sendStream.WriteAsync(buffer, 0, count));
            }

            await Task.WhenAll(tasks);
        }
    }
}
