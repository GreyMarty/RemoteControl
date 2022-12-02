namespace RemoteControl.Server
{
    public interface ICommandResolver<TCommand, TFunc> 
        where TCommand : Enum
        where TFunc : Delegate
    {
        public void Bind(TCommand command, TFunc func);

        public void Unbind(TCommand command);

        public TFunc Resolve(TCommand command);
    }
}
