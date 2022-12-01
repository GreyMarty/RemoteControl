using System.Net;
using System.Net.Sockets;

namespace RemoteControl.Server
{
    public interface IRouter
    {
        public void Bind(EndPoint to, EndPoint what);

        public void Unbind(EndPoint from, EndPoint what);

        public IReadOnlyCollection<EndPoint> Resolve(EndPoint endPoint);

        public IReadOnlyCollection<EndPoint> GetAll();
    }
}
