namespace RemoteControl
{
	namespace IO
	{
		namespace Mouse
		{
			namespace Manipulation
			{
				inline int MouseMoveCommand::DataSize::get() 
				{
					return sizeof(Core::PointI) + sizeof(bool);
				}

				inline Core::PointI MouseMoveCommand::Delta::get()
				{
					return m_delta;
				}

				inline void MouseMoveCommand::Delta::set(Core::PointI delta)
				{
					m_delta = delta;
				}

				inline bool MouseMoveCommand::IsAbsolute::get()
				{
					return m_isAbsolute;
				}

				inline void MouseMoveCommand::IsAbsolute::set(bool isAbsolute)
				{
					m_isAbsolute = isAbsolute;
				}
			}
		}
	}
}