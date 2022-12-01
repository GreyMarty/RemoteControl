using System.Net;
using System.Net.Sockets;

namespace RemoteControl.Server
{
    public class Server
    {
        private ICommandListener _commandListener;
        private Thread _commandListenerThread;

        private TcpListener _videoListener;
        private TcpListener _controlListener;

        private Dictionary<EndPoint, Socket> _clients;
        private Dictionary<string, EndPoint> _endPoints;


        public int UdpPort => _commandListener.Port;


        public Server(int udpPort, int tcpPort) 
        {
            _clients = new Dictionary<EndPoint, Socket>();
            _endPoints = new Dictionary<string, EndPoint>();

            _commandListener = new CommandListener(
                udpPort,
                _clients,
                _endPoints,
                new Router(),
                new NetCommandResolver<CommandListener.ProcessCommandCallback>(),
                new ConnectionStringProvider()
            );

            //_videoListener = new TcpListener(IPAddress.Any, tcpPort);
            //_controlListener = new TcpListener(IPAddress.Loopback, tcpPort + 1);
        }

        public void Run() 
        {
            _commandListenerThread = new Thread(_commandListener.Run);
            _commandListenerThread.Start();
        }
    }
}
