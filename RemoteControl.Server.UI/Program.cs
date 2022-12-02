using log4net;
using RemoteControl.Server;


var logger = LogManager.GetLogger(System.Reflection.MethodBase.GetCurrentMethod().DeclaringType);
var server = new Server(40040, 40040, logger);

server.Run();

Console.WriteLine($"UDP Port: {server.UdpPort}");
Console.WriteLine($"TCP Port: {server.TcpPort}");
