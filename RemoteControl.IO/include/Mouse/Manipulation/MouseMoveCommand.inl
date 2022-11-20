namespace RemoteControl
{
	namespace IO
	{
		namespace Mouse
		{
			namespace Manipulation
			{
				inline Core::PointI MouseMoveCommand::Delta::get()
				{
					return m_delta;
				}

				inline void MouseMoveCommand::Delta::set(Core::PointI delta)
				{
					m_delta = delta;
				}
			}
		}
	}
}