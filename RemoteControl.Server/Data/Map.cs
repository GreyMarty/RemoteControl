using System.Collections;
using System.Diagnostics.CodeAnalysis;


namespace RemoteControl.Server.Data
{
    public class Map<TKey, TValue> : IMap<TKey, TValue>
        where TKey : notnull
        where TValue : notnull
    {
        private Dictionary<TKey, TValue> _forward;
        private Dictionary<TValue, TKey> _reverse;


        public IDictionary<TKey, TValue> Forward { get; init; }
        public IDictionary<TValue, TKey> Reverse { get; init; }

        public int Count => _forward.Count;


        public Map()
        {
            _forward = new Dictionary<TKey, TValue>();
            _reverse = new Dictionary<TValue, TKey>();

            Forward = new Indexer<TKey, TValue>(_forward, _reverse);
            Reverse = new Indexer<TValue, TKey>(_reverse, _forward);
        }

        private class Indexer<TIKey, TIValue> : IDictionary<TIKey, TIValue>
        {
            IDictionary<TIKey, TIValue> _forward;
            IDictionary<TIValue, TIKey> _reverse;


            public int Count => _forward.Count;

            public ICollection<TIKey> Keys => _forward.Keys;

            public ICollection<TIValue> Values => _forward.Values;

            public bool IsReadOnly => _forward.IsReadOnly;

            TIValue IDictionary<TIKey, TIValue>.this[TIKey key] { get => this[key]; set => this[key] = value; }

            public TIValue this[TIKey key]
            {
                get => _forward[key];
                set 
                {
                    if (_forward.ContainsKey(key))
                    {
                        _reverse.Remove(_forward[key]);
                    }

                    _forward[key] = value;
                    _reverse[value] = key;
                }
            }


            public Indexer(IDictionary<TIKey, TIValue> forward, IDictionary<TIValue, TIKey> reverse) 
            {
                _forward = forward;
                _reverse = reverse;
            }

            public void Add(TIKey key, TIValue value) 
            {
                _forward.Add(key, value);
                _reverse.Add(value, key);
            }

            public void Add(KeyValuePair<TIKey, TIValue> item)
            {
                this[item.Key] = item.Value;
            }

            public bool Remove(TIKey key) 
            {
                if (!_forward.ContainsKey(key)) 
                {
                    return false;
                }

                _reverse.Remove(_forward[key]);
                _forward.Remove(key);

                return true;
            }

            public bool Remove(KeyValuePair<TIKey, TIValue> item)
            {
                if (!_forward.Contains(item)) 
                {
                    return false;
                }

                return Remove(item.Key);
            }

            public bool ContainsKey(TIKey key)
            {
                return _forward.ContainsKey(key);
            }

            public bool TryGetValue(TIKey key, [MaybeNullWhen(false)] out TIValue value)
            {
                return _forward.TryGetValue(key, out value);
            }

            public void Clear()
            {
                _forward.Clear();
                _reverse.Clear();
            }

            public bool Contains(KeyValuePair<TIKey, TIValue> item)
            {
                return _forward.Contains(item);
            }

            public void CopyTo(KeyValuePair<TIKey, TIValue>[] array, int arrayIndex)
            {
                _forward.CopyTo(array, arrayIndex);
            }

            public IEnumerator<KeyValuePair<TIKey, TIValue>> GetEnumerator()
            {
                return _forward.GetEnumerator();
            }

            IEnumerator IEnumerable.GetEnumerator()
            {
                return GetEnumerator();
            }
        }
    }
}
