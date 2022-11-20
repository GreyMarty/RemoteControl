#pragma once

#include <inttypes.h>

#include "Keyboard/KeyboardKey.hpp"
#include "Manipulation/ICommand.hpp"

namespace RemoteControl
{
	namespace IO
	{
		namespace Keyboard
		{
			namespace Manipulation
			{
				public ref class KeyboardKeyCommand : public IO::Manipulation::ICommand
				{
				public:
					virtual property int DataSize
					{
						int get();
					}

					property KeyboardKey Key 
					{
						KeyboardKey get();
						void set(KeyboardKey value);
					}

					property KeyboardKeyEvent KeyEvent 
					{
						KeyboardKeyEvent get();
						void set(KeyboardKeyEvent value);
					}


					virtual void Execute();

					virtual int WriteTo(System::IO::Stream^ stream);

					static KeyboardKeyCommand^ Parse(System::IO::Stream^ stream);

				private:
					KeyboardKey m_key;
					KeyboardKeyEvent m_keyEvent;
				};
			}
		}
	}
}

#include "KeyboardKeyCommand.inl"