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

					property uint32_t Scancode
					{
						uint32_t get();
						void set(uint32_t value);
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
					uint32_t m_scanCode;
					KeyboardKeyEvent m_keyEvent;
				};
			}
		}
	}
}

#include "KeyboardKeyCommand.inl"