using System.Threading;
using RemoteControl.Core;
using RemoteControl.IO;


for (int i = 0; i < 100; i++) 
{
    //Mouse.Move(new PointI(1, 1));
    Mouse.Scroll(-1000);
    Thread.Sleep(1);
}