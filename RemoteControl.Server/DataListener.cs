using RemoteControl.Net.Enums;
using RemoteControl.Server.Data;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace RemoteControl.Server
{
    public class DataListener : IDataListener
    {
        private const int Timeout = 2000;
        private const int BufferSize = 4096;

        private readonly TcpListener _tcpListener;

        private readonly IRouter _router;
        private readonly IMap<string, EndPoint> _udpEndPoints;
        private readonly IMap<string, EndPoint> _tcpEndPoints;

        private readonly IDictionary<EndPoint, Socket> _sockets;
        private readonly IDictionary<EndPoint, Stream> _netStreams;

        private Thread _dataThread;
        private Mutex _streamMutex;


        public bool IsRunning { get; private set; }

        public int Port => (_tcpListener?.LocalEndpoint as IPEndPoint)?.Port ?? 0;


        public DataListener(
            IPAddress iPAddress,
            int port,
            IRouter router,
            IMap<string, EndPoint> udpEndPoints,
            IMap<string, EndPoint> tcpEndPoints
        )
        {
            _tcpListener = new TcpListener(iPAddress, port);
            _tcpListener.Server.ReceiveTimeout = Timeout;
            _tcpListener.Server.SendTimeout = Timeout;

            _router = router;
            _udpEndPoints = udpEndPoints;
            _tcpEndPoints = tcpEndPoints;

            _sockets = new Dictionary<EndPoint, Socket>();
            _netStreams = new Dictionary<EndPoint, Stream>();
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

        private void Listen() 
        {
            while (IsRunning) 
            {
                var tasks = new List<Task>();

                if (_netStreams.Count < 2) 
                {
                    continue;
                }

                _streamMutex.WaitOne();

                foreach (var endPoint in _netStreams.Keys) 
                {
                    tasks.Add(Redirect(endPoint));
                }

                _streamMutex.ReleaseMutex();

                Task.WaitAll(tasks.ToArray());
            }
        }

        private async Task ProcessAcceptedClientAsync(Socket socket) 
        {
            var connectionStringBytes = new byte[8];
            await socket.ReceiveAsync(connectionStringBytes, SocketFlags.None);

            var connectionsString = Encoding.UTF8.GetString(connectionStringBytes);

            if (!_udpEndPoints.Forward.ContainsKey(connectionsString)) 
            {
                await socket.SendAsync(new byte[] { (byte)NetResult.Error }, SocketFlags.None);
                return;
            }

            _tcpEndPoints.Forward[connectionsString] = socket.RemoteEndPoint;
            _sockets[socket.RemoteEndPoint] = socket;

            _streamMutex.WaitOne();
            _netStreams[socket.RemoteEndPoint] = new NetworkStream(socket);
            _streamMutex.ReleaseMutex();

            await socket.SendAsync(new byte[] { (byte)NetResult.Ok }, SocketFlags.None);
        }

        private async Task Redirect(EndPoint endPoint) 
        {
            var stream = _netStreams[endPoint];
            var connectionString = _tcpEndPoints.Reverse[endPoint];

            var buffer = new byte[BufferSize];
            var readTask = stream.ReadAsync(buffer, 0, buffer.Length);

            await Task.WhenAny(readTask, Task.Delay(Timeout));

            if (!readTask.IsCompleted) 
            {
                return;
            }

            var tasks = new List<Task>();

            foreach (var sendConnectionString in _router.Resolve(connectionString))
            {
                var sendEndPoint = _tcpEndPoints.Forward[sendConnectionString];
                var sendStream = _netStreams[sendEndPoint];

                tasks.Add(Task.WhenAny(sendStream.WriteAsync(buffer, 0, buffer.Length), Task.Delay(Timeout)));
            }

            await Task.WhenAll(tasks);
        }
    }
}
