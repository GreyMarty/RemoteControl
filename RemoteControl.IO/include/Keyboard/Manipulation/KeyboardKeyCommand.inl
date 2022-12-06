namespace RemoteControl
{
	namespace IO
	{
		namespace Keyboard
		{
			namespace Manipulation
			{
				inline int KeyboardKeyCommand::DataSize::get() 
				{
					return sizeof(uint8_t) + sizeof(uint32_t);
				}

				inline uint32_t KeyboardKeyCommand::Scancode::get() 
				{
					return m_scanCode;
				}

				inline void KeyboardKeyCommand::Scancode::set(uint32_t value) 
				{
					m_scanCode = value;
				}

				inline KeyboardKeyEvent KeyboardKeyCommand::KeyEvent::get()
				{
					return m_keyEvent;
				}

				inline void KeyboardKeyCommand::KeyEvent::set(KeyboardKeyEvent value)
				{
					m_keyEvent = value;
				}
			}
		}
	}
}
