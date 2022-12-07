using log4net;
using RemoteControl.Server.Data;
using System.Net;

namespace RemoteControl.Server
{
    public class Server
    {
        private readonly ILog _logger;

        private CommandListener _commandListener;
        private Thread _commandListenerThread;

        private DataListener _dataListener;
        private Thread _dataListenerThread;

        private Map<string, EndPoint> _udpEndPoints;
        private Map<string, EndPoint> _tcpEndPoints;

        private IRouter _router;


        public int UdpPort => _commandListener.Port;
        public int TcpPort => _dataListener.Port;


        public Server(int udpPort, int tcpPort, ILog logger) 
        {
            _tcpEndPoints = new Map<string, EndPoint>();
            _udpEndPoints = new Map<string, EndPoint>();

            _router = new Router();

            _commandListener = new CommandListener(
                udpPort,
                _udpEndPoints,
                _router,
                new NetCommandResolver<CommandListener.ProcessCommandCallback>(),
                new ConnectionStringProvider(),
                logger
            );

            _commandListener.ClientForsakenControl += _commandListener_ClientForsakenControl;
            _commandListener.ClientDisconnected += _commandListener_ClientDisconnected;

            _dataListener = new DataListener(
                IPAddress.Any, tcpPort,
                _router,
                _udpEndPoints,
                _tcpEndPoints,
                logger
            );

            _logger = logger;
        }

        private async Task _commandListener_ClientDisconnected(object sender, IPEndPoint endPoint)
        {
            _dataListener.DisconnectInactive();
        }

        private async Task _commandListener_ClientForsakenControl(object sender, IPEndPoint hostEndPoint, IPEndPoint clientEndPoint)
        {
            _dataListener?.DisconnectInactive();
        }

        public void Run() 
        {
            _logger.Info("server started...");

            _commandListenerThread = new Thread(_commandListener.Run);
            _commandListenerThread.Start();

            _dataListenerThread = new Thread(_dataListener.Run);
            _dataListenerThread.Start();
        }
    }
}
