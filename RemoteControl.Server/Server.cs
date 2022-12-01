using RemoteControl.Server.Data;
using System.Net;

namespace RemoteControl.Server
{
    public class Server
    {
        private ICommandListener _commandListener;
        private Thread _commandListenerThread;

        private IDataListener _dataListener;
        private Thread _dataListenerThread;

        private Map<string, EndPoint> _udpEndPoints;
        private Map<string, EndPoint> _tcpEndPoints;

        private IRouter _router;


        public int UdpPort => _commandListener.Port;
        public int TcpPort => _dataListener.Port;


        public Server(int udpPort, int tcpPort) 
        {
            _tcpEndPoints = new Map<string, EndPoint>();
            _udpEndPoints = new Map<string, EndPoint>();

            _router = new Router();

            _commandListener = new CommandListener(
                udpPort,
                _udpEndPoints,
                _router,
                new NetCommandResolver<CommandListener.ProcessCommandCallback>(),
                new ConnectionStringProvider()
            );

            _dataListener = new DataListener(
                IPAddress.Any, 0,
                _router,
                _udpEndPoints,
                _tcpEndPoints
            );
        }

        public void Run() 
        {
            _commandListenerThread = new Thread(_commandListener.Run);
            _commandListenerThread.Start();

            _dataListenerThread = new Thread(_dataListener.Run);
            _dataListenerThread.Start();
        }
    }
}
