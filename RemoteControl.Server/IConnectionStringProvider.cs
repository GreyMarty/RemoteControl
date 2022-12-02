namespace RemoteControl.Server
{
    public interface IConnectionStringProvider
    {
        string Generate();

        void Release(string connectionString);
    }
}
