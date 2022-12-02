using System.Text;
using System.Net;
using System.Net.Sockets;
using RemoteControl.Net.Enums;

namespace RemoteControl.Client
{
    public class Client
    {
        private const int Timeout = 2000;

        private readonly UdpClient _udpClient;
        private readonly TcpClient _tcpClient;

        private readonly IPEndPoint _udpEndPoint;
        private readonly IPEndPoint _tcpEndPoint;

        private bool _isListening;
        private Thread _listenThread;


        public string ConnectionString { get; private set; }

        public Stream NetworkStream { get; private set; }


        public Client(IPAddress iPAddress, int updPort, int tcpPort)
        {
            _udpClient = new UdpClient();
            _udpClient.Client.ReceiveTimeout = Timeout;
            _udpClient.Client.SendTimeout = Timeout;

            _tcpClient = new TcpClient();

            _udpEndPoint = new IPEndPoint(iPAddress, updPort);
            _tcpEndPoint = new IPEndPoint(iPAddress, tcpPort);
        }

        public async Task<NetResult> ConnectAsync() 
        {
            var sendTask = _udpClient.SendAsync(new byte[] { (byte)NetCommand.Connect }, 1, _udpEndPoint);
            await Task.WhenAny(sendTask, Task.Delay(Timeout));

            var recieveTask = _udpClient.ReceiveAsync();
            await Task.WhenAny(recieveTask, Task.Delay(Timeout));

            if (!recieveTask.IsCompleted) 
            {
                return NetResult.Error;
            }

            var buffer = recieveTask.Result.Buffer;

            if (buffer.Length == 0 || buffer[0] != (byte)NetResult.Ok)
            {
                return (NetResult)buffer[0];
            }

            ConnectionString = Encoding.UTF8.GetString(buffer, 1, buffer.Length - 1);

            StartListening();

            return NetResult.Ok;
        }

        public async Task<NetResult> TakeControlAsync(string connectionString) 
        {
            StopListening();
            // Wait until listening thread dies
            // Probably needs fixing
            await Task.Delay(Timeout);

            var streamBuffer = new MemoryStream();
            streamBuffer.WriteByte((byte)NetCommand.TakeControl);
            streamBuffer.Write(Encoding.UTF8.GetBytes(connectionString));

            var sendTask = _udpClient.SendAsync(streamBuffer.ToArray(), (int)streamBuffer.Length, _udpEndPoint);
            await Task.WhenAny(sendTask, Task.Delay(Timeout));

            if (!sendTask.IsCompleted) 
            {
                StartListening();
                return NetResult.Error;
            }

            var recieveTask = _udpClient.ReceiveAsync();
            await Task.WhenAny(recieveTask, Task.Delay(Timeout));

            if (!recieveTask.IsCompleted) 
            {
                StartListening();
                return NetResult.Error;
            }

            var buffer = recieveTask.Result.Buffer;

            if (buffer.Length == 0 || buffer[0] != (byte)NetResult.Ok) 
            {
                StartListening();
                return NetResult.Error;
            }

            return await ConnectTcpAsync();
        }

        private async Task<NetResult> ConnectTcpAsync() 
        {
            _tcpClient.Connect(_tcpEndPoint);

            var stream = _tcpClient.GetStream();
            await stream.WriteAsync(Encoding.UTF8.GetBytes(ConnectionString));

            var response = new byte[1];
            await stream.ReadAsync(response, 0, 1);

            if (response[0] == (byte)NetResult.Ok)
            {
                NetworkStream = stream;
            }

            return (NetResult)response[0];
        }

        private void StartListening() 
        {
            _listenThread = new Thread(Listen);
            _listenThread.Start();
        }

        private void StopListening() 
        {
            _isListening = false;
        }

        private void Listen() 
        {
            _isListening = true;

            while (_isListening) 
            {
                byte[] buffer = null;
                IPEndPoint endPoint = null;

                try
                {
                    buffer = _udpClient.Receive(ref endPoint);
                }
                catch (SocketException e) 
                {
                    if (e.ErrorCode != 10060) 
                    {
                        throw e;
                    }
                }

                if (buffer is not null) 
                {
                    _udpClient.Send(new byte[] { (byte)NetResult.Ok }, _udpEndPoint);
                    _isListening = false;

                    Task.WaitAny(Task.Run(ConnectTcpAsync), Task.Delay(Timeout));
                    return;
                }
            }
        }
    }
}
