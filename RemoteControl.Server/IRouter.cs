namespace RemoteControl.Server
{
    public interface IRouter
    {
        public void Bind(string to, string what);

        public void Unbind(string from, string what);

        public IReadOnlyCollection<string> Resolve(string connectionString);

        public IReadOnlyCollection<string> GetAll();
    }
}
