using System.Text;
using System.Net;
using System.Net.Sockets;
using RemoteControl.Net.Enums;
using System.Diagnostics;

namespace RemoteControl.Client
{
    public class Client
    {
        private const int Timeout = 1000;

        private readonly UdpClient _udpClient;
        private TcpClient _tcpClient;

        private readonly IPEndPoint _udpEndPoint;
        private readonly IPEndPoint _tcpEndPoint;

        private bool _isListening;
        private Thread _listenThread;


        public bool IsConnected { get; private set; }

        public string ConnectionString { get; private set; }

        public Stream NetworkStream { get; private set; }


        public delegate void TakenControlHandler(object sender, string connectionString);
        public event TakenControlHandler TakenControl;

        public delegate void ForsakenControlHandler(object sender);
        public event ForsakenControlHandler ForsakenControl;


        public Client(IPAddress iPAddress, int updPort, int tcpPort)
        {
            _udpClient = new UdpClient();
            _udpClient.Client.ReceiveTimeout = Timeout;
            _udpClient.Client.SendTimeout = Timeout;

            _udpEndPoint = new IPEndPoint(iPAddress, updPort);
            _tcpEndPoint = new IPEndPoint(iPAddress, tcpPort);
        }

        public async Task<NetResult> ConnectAsync() 
        {
            var sendTask = _udpClient.SendAsync(new byte[] { (byte)NetCommand.Connect }, 1, _udpEndPoint);
            await Task.WhenAny(sendTask, Task.Delay(Timeout));

            var recieveTask = _udpClient.ReceiveAsync();
            await Task.WhenAny(recieveTask, Task.Delay(Timeout));

            if (!recieveTask.IsCompleted || !recieveTask.IsCompletedSuccessfully) 
            {
                return NetResult.Error;
            }

            var buffer = recieveTask.Result.Buffer;

            if (buffer.Length == 0 || buffer[0] != (byte)NetResult.Ok)
            {
                return (NetResult)buffer[0];
            }

            ConnectionString = Encoding.UTF8.GetString(buffer, 1, buffer.Length - 1);

            IsConnected = true;
            StartListening();

            return NetResult.Ok;
        }

        public async Task DisconnectAsync() 
        {
            await ForsakeControlAsync();

            await Task.WhenAny(
                _udpClient.SendAsync(new byte[] { (byte)NetCommand.Disconnect }, 1, _udpEndPoint),
                Task.Delay(Timeout)
            ); ;
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

            if (!sendTask.IsCompleted || !sendTask.IsCompletedSuccessfully) 
            {
                StartListening();
                return NetResult.Error;
            }

            var recieveTask = _udpClient.ReceiveAsync();
            await Task.WhenAny(recieveTask, Task.Delay(Timeout));

            if (!recieveTask.IsCompleted || !recieveTask.IsCompletedSuccessfully) 
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

            StartListening();
            return await ConnectTcpAsync();
        }

        public async Task ForsakeControlAsync() 
        {
            await Task.WhenAny(
                _udpClient.SendAsync(new byte[] { (byte)NetCommand.ForsakeControl }, 1, _udpEndPoint),
                Task.Delay(Timeout)
            );

            CloseTcpConnection();
        }

        private void NotifyForsakenControl() 
        {
            ForsakenControl?.Invoke(this);
            CloseTcpConnection();
        }

        private void CloseTcpConnection() 
        {
            if (_tcpClient?.Connected ?? false)
            {
                _tcpClient?.Close();
            }

            NetworkStream = null;
        }

        private async Task<NetResult> ConnectTcpAsync()
        {
            if (_tcpClient?.Connected ?? false) 
            {
                return NetResult.Ok;
            }

            _tcpClient = new TcpClient();
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
            if (IsConnected)
            {
                _listenThread = new Thread(Listen);
                _listenThread.Start();
            }
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
                    try
                    {
                        switch ((NetCommand)buffer[0])
                        {
                            case NetCommand.TakeControl:
                                _udpClient.Send(new byte[] { (byte)NetResult.Ok }, _udpEndPoint);
                                Task.WaitAny(Task.Run(ConnectTcpAsync), Task.Delay(Timeout));
                                TakenControl?.Invoke(this, Encoding.UTF8.GetString(buffer, 1, buffer.Length - 1));
                                break;
                            case NetCommand.ForsakeControl:
                                NotifyForsakenControl();
                                break;
                        };

                    }
                    catch (Exception e)
                    {
                        Debug.WriteLine($"Listen: {e}");
                    }
                }
            }
        }
    }
}
