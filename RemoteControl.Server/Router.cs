namespace RemoteControl.Server
{
    public class Router : IRouter
    {
        private Dictionary<string, HashSet<string>> _routingTable;


        public Router()
        {
            _routingTable = new Dictionary<string, HashSet<string>>();
        }

        public void Bind(string to, string what) 
        {
            if (!_routingTable.ContainsKey(to)) 
            {
                _routingTable.Add(to, new HashSet<string>());
            }

            if (!_routingTable.ContainsKey(what)) 
            {
                _routingTable.Add(what, new HashSet<string>());
            }

            _routingTable[to].Add(what);
            _routingTable[what].Add(to);
        }

        public void Unbind(string from, string what) 
        {
            if (!_routingTable.ContainsKey(from)) 
            {
                return;
            }

            _routingTable[from].Remove(what);

            if (!_routingTable.ContainsKey(what))
            {
                return;
            }

            _routingTable[what].Remove(from);
        }

        public IReadOnlyCollection<string> Resolve(string connectionString) 
        {
            if (!_routingTable.ContainsKey(connectionString)) 
            {
                return Array.Empty<string>();
            }

            return _routingTable[connectionString];
        }

        public IReadOnlyCollection<string> GetAll() 
        {
            return _routingTable.Keys;
        }
    }
}
