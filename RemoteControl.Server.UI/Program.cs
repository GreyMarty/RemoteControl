using log4net;
using RemoteControl.Server;
using System.Configuration;

var logger = LogManager.GetLogger(System.Reflection.MethodBase.GetCurrentMethod().DeclaringType);
var server = new Server(
    int.Parse(ConfigurationManager.AppSettings["UdpPort"]),
    int.Parse(ConfigurationManager.AppSettings["TcpPort"]),
    logger
);

server.Run();

Console.WriteLine($"UDP Port: {server.UdpPort}");
Console.WriteLine($"TCP Port: {server.TcpPort}");
