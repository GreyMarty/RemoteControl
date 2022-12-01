using System.Net;

namespace RemoteControl.Server
{
    public class Router : IRouter
    {
        private Dictionary<EndPoint, HashSet<EndPoint>> _routingTable;


        public Router()
        {
            _routingTable = new Dictionary<EndPoint, HashSet<EndPoint>>();
        }

        public void Bind(EndPoint to, EndPoint what) 
        {
            if (!_routingTable.ContainsKey(to)) 
            {
                _routingTable.Add(to, new HashSet<EndPoint>());
            }

            _routingTable[to].Add(what);
        }

        public void Unbind(EndPoint from, EndPoint what) 
        {
            if (!_routingTable.ContainsKey(from)) 
            {
                return;
            }

            _routingTable[from].Remove(what);
        }

        public IReadOnlyCollection<EndPoint> Resolve(EndPoint endPoint) 
        {
            if (!_routingTable.ContainsKey(endPoint)) 
            {
                return null;
            }

            return _routingTable[endPoint];
        }

        public IReadOnlyCollection<EndPoint> GetAll() 
        {
            return _routingTable.Keys;
        }
    }
}
