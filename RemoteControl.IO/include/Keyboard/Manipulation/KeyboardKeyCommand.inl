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
					return sizeof(int) * 2;
				}

				inline KeyboardKey KeyboardKeyCommand::Key::get() 
				{
					return m_key;
				}

				inline void KeyboardKeyCommand::Key::set(KeyboardKey value) 
				{
					m_key = value;
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
