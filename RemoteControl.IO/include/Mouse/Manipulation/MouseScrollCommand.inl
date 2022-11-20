namespace RemoteControl
{
	namespace IO
	{
		namespace Mouse
		{
			namespace Manipulation
			{
				inline int MouseScrollCommand::Delta::get()
				{
					return m_delta;
				}

				inline void MouseScrollCommand::Delta::set(int delta)
				{
					m_delta = delta;
				}
			}
		}
	}
}
