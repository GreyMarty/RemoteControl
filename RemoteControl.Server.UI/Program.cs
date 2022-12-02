using log4net;
using RemoteControl.Server;


var logger = LogManager.GetLogger(System.Reflection.MethodBase.GetCurrentMethod().DeclaringType);
var server = new Server(0, 0, logger);

server.Run();
