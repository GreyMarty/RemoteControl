namespace RemoteControl.Server.Data
{
    public interface IMap<TKey, TValue>
        where TKey : notnull
        where TValue : notnull
    {
        public IDictionary<TKey, TValue> Forward { get; }
        public IDictionary<TValue, TKey> Reverse { get; }

        public int Count { get; }
    }
}
