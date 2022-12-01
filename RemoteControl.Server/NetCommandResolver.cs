using RemoteControl.Net.Enums;

namespace RemoteControl.Server
{
    internal class NetCommandResolver<TFunc> : ICommandResolver<NetCommand, TFunc>
        where TFunc : Delegate
    {
        private Dictionary<NetCommand, TFunc> _commands;


        public NetCommandResolver()
        {
            _commands = new Dictionary<NetCommand, TFunc>();
        }

        public void Bind(NetCommand command, TFunc func)
        {
            _commands[command] = func;
        }

        public void Unbind(NetCommand command)
        {
            _commands.Remove(command);
        }

        public TFunc Resolve(NetCommand command)
        {
            if (!_commands.ContainsKey(command)) 
            {
                return null;
            }

            return _commands[command];
        }
    }
}
