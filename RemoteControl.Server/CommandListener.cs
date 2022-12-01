using System.Net;
using System.Net.Sockets;
using System.Text;
using RemoteControl.Net.Enums;
using RemoteControl.Server.Data;

namespace RemoteControl.Server
{
    public class CommandListener : ICommandListener
    {
        private const int Timeout = 2000;
        
        private readonly IRouter _router;
        private readonly ICommandResolver<NetCommand, ProcessCommandCallback> _commandResolver;
        private readonly IConnectionStringProvider _connectionStringProvider;

        private readonly IMap<string, EndPoint> _udpEndPoints;

        private readonly UdpClient _udpClient;

        
        public bool IsRunning { get; private set; }

        public int Port { get; init; } 


        public delegate Task ClientConnectedHandler(object sender, IPEndPoint endPoint);
        public delegate Task ClientDisconnectedHandler(object sender, IPEndPoint endPoint);
        public delegate Task ClientTookControlHandler(object sender, IPEndPoint hostEndPoint, IPEndPoint clientEndPoint);
        public delegate Task ClientForsakenControlHandler(object sender, IPEndPoint hostEndPoint, IPEndPoint clientEndPoint);

        public delegate Task ProcessCommandCallback(IPEndPoint endPoint, byte[] buffer);


        public event ClientConnectedHandler ClientConnected;
        public event ClientDisconnectedHandler ClientDisconnected;
        public event ClientTookControlHandler ClientTookControl;
        public event ClientForsakenControlHandler ClientForsakenControl;


        public CommandListener(
            int port, 
            // Dependencies
            IMap<string, EndPoint> _updEndPoints,
            IRouter router,
            ICommandResolver<NetCommand, ProcessCommandCallback> commandResolver, 
            IConnectionStringProvider connectionStringProvider
        )
        {
            _udpClient = new UdpClient(port);
            _udpClient.Client.ReceiveTimeout = Timeout;
            _udpClient.Client.SendTimeout = Timeout;

            Port = ((IPEndPoint)_udpClient.Client.LocalEndPoint).Port;

            _udpEndPoints = _updEndPoints;
            _router = router;
            _commandResolver = commandResolver;
            _connectionStringProvider = connectionStringProvider;

            BindCommands();
        }

        public void Dispose()
        {
            _udpClient?.Dispose();
        }

        public void Run()
        {
            IsRunning = true;

            var tasks = new List<Task>();

            while (IsRunning) 
            {
                IPEndPoint endPoint = null;
                byte[] resultBuffer = null;

                try
                {
                    resultBuffer = _udpClient.Receive(ref endPoint);

                }
                catch (SocketException e) 
                {
                    // not WSATIMEDOUT
                    if (e.ErrorCode != 10060) 
                    {
                        throw e;
                    }
                }

                if (resultBuffer is not null) 
                {
                    tasks.Add(ProcessCommandAsync(endPoint, resultBuffer));
                }
            }
        }

        public void Close() 
        {
            IsRunning = false;
        }

        private void BindCommands() 
        {
            _commandResolver.Bind(NetCommand.Connect, ProcessConnectCommandAsync);
            _commandResolver.Bind(NetCommand.Disconnect, ProcessDisconnectCommandAsync);
            _commandResolver.Bind(NetCommand.TakeControl, ProcessTakeControlCommandAsync);
            _commandResolver.Bind(NetCommand.ForsakeControl, ProcessForsakeControlCommandAsync);
        }

        private async Task ProcessCommandAsync(IPEndPoint endPoint, byte[] buffer) 
        {
            var command = (NetCommand)buffer[0];
            var task = _commandResolver.Resolve(command)?.Invoke(endPoint, buffer);

            if (task is not null) 
            {
                await task;
            }
        }

        private async Task ProcessConnectCommandAsync(IPEndPoint endPoint, byte[] buffer) 
        {
            string connectionString;

            if (!_udpEndPoints.Reverse.ContainsKey(endPoint))
            {
                connectionString = _connectionStringProvider.Generate();
                _udpEndPoints.Forward.Add(connectionString, endPoint);
            }
            else 
            {
                connectionString = _udpEndPoints.Reverse[endPoint];
            }

            var reply = new MemoryStream();
            reply.WriteByte((byte)NetResult.Ok);
            reply.Write(Encoding.UTF8.GetBytes(connectionString));

            var tasks = new List<Task>();

            var task = ClientConnected?.Invoke(this, endPoint);

            if (task is not null) 
            {
                tasks.Add(task);
            }

            tasks.Add(_udpClient.SendAsync(reply.ToArray(), (int)reply.Length, endPoint));

            await Task.WhenAll(tasks);
        }

        private async Task ProcessDisconnectCommandAsync(IPEndPoint endPoint, byte[] buffer) 
        {
            var result = _udpEndPoints.Reverse.Remove(endPoint) ? NetResult.Ok : NetResult.Error;

            var tasks = new List<Task>();

            if (result == NetResult.Ok)
            {
                var connectionString = _udpEndPoints.Reverse[endPoint];
                _udpEndPoints.Forward.Remove(connectionString);

                var task = ClientDisconnected?.Invoke(this, endPoint);

                if (task is not null) 
                {
                    tasks.Add(task);
                }
            }

            tasks.Add(_udpClient.SendAsync(new byte[] { (byte)result }, 1, endPoint));

            await Task.WhenAll(tasks);
        }

        private async Task ProcessTakeControlCommandAsync(IPEndPoint endPoint, byte[] buffer) 
        {
            var connectionString = Encoding.UTF8.GetString(buffer, 1, buffer.Length - 1);

            var result = NetResult.Error;

            var tasks = new List<Task>();

            if (_udpEndPoints.Forward.ContainsKey(connectionString)) 
            {
                var hostEndPoint = _udpEndPoints.Forward[connectionString];
                
                _router.Bind(connectionString, _udpEndPoints.Reverse[endPoint]);

                result = NetResult.Ok;

                var task = ClientTookControl?.Invoke(this, (IPEndPoint)hostEndPoint, endPoint);

                if (task is not null) 
                {
                    tasks.Add(task);
                }
            }

            tasks.Add(_udpClient.SendAsync(new byte[] { (byte)result }, 1, endPoint));

            await Task.WhenAll(tasks);
        }

        private async Task ProcessForsakeControlCommandAsync(IPEndPoint endPoint, byte[] buffer) 
        {
            var connectionString = Encoding.UTF8.GetString(buffer, 1, buffer.Length - 1);

            var result = NetResult.Error;

            var tasks = new List<Task>();

            if (_udpEndPoints.Forward.ContainsKey(connectionString)) 
            {
                var hostEndPoint = _udpEndPoints.Forward[connectionString];
                
                _router.Unbind(connectionString, _udpEndPoints.Reverse[endPoint]);

                result = NetResult.Ok;

                var task = ClientForsakenControl?.Invoke(this, (IPEndPoint)hostEndPoint, endPoint);

                if (task is not null) 
                {
                    tasks.Add(task);
                }
            }

            tasks.Add(_udpClient.SendAsync(new byte[] { (byte)result }, 1, endPoint));

            await Task.WhenAll(tasks);
        }
    }
}
