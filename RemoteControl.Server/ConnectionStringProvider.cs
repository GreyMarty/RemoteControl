using System.Security.Cryptography;

namespace RemoteControl.Server
{
    public class ConnectionStringProvider : IConnectionStringProvider
    {
        private const int DefaultStringLength = 8;

        private HashSet<int> _used;

        private RandomNumberGenerator _rand;

        private readonly int _stringLength;


        public ConnectionStringProvider(int connectionStringLength=DefaultStringLength)
        {
            _used = new HashSet<int>();
            _rand = RandomNumberGenerator.Create();
            _stringLength = connectionStringLength;
        }

        public string Generate()
        {
            int hash;

            var connectionStringBytes = new byte[(_stringLength >> 2) * 3];

            do
            {
                _rand.GetBytes(connectionStringBytes);
                hash = connectionStringBytes.GetHashCode();
            } while (_used.Contains(hash));

            _used.Add(connectionStringBytes.GetHashCode());
            
            return Convert.ToBase64String(connectionStringBytes);
        }

        public void Release(string connectionString) 
        {
            int hash = Convert.FromBase64String(connectionString).GetHashCode();
            _used.Remove(hash);
        }
    }
}
