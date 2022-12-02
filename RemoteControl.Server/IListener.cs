namespace RemoteControl.Server
{
    public interface IListener: IDisposable
    {
        public bool IsRunning { get; }

        public int Port { get; }


        public void Run();

        public void Close();
    }
}
