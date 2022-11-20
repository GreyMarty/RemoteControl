namespace RemoteControl 
{
	namespace IO 
	{
		namespace Mouse
		{
			namespace Manipulation
			{
				inline MouseButton MouseButtonCommand::Button::get()
				{
					return m_button;
				}

				inline void MouseButtonCommand::Button::set(MouseButton button)
				{
					m_button = button;
				}

				inline MouseButtonEvent MouseButtonCommand::ButtonEvent::get()
				{
					return m_buttonEvent;
				}

				inline void MouseButtonCommand::ButtonEvent::set(MouseButtonEvent buttonEvent)
				{
					m_buttonEvent = buttonEvent;
				}
			}
		}
	}
}
