#pragma once

#include <inttypes.h>

#include "Manipulation/ICommand.hpp"
#include "Mouse/Mouse.hpp"

namespace RemoteControl
{
	namespace IO
	{
		namespace Mouse
		{
			namespace Manipulation
			{
				public ref class MouseButtonCommand : public IO::Manipulation::ICommand
				{
				public:
					virtual property int DataSize 
					{
						int get();
					}

					property MouseButton Button
					{
						MouseButton get();
						void set(MouseButton button);
					}

					property MouseButtonEvent ButtonEvent
					{
						MouseButtonEvent get();
						void set(MouseButtonEvent buttonEvent);
					}


					virtual void Execute();

					virtual int WriteTo(System::IO::Stream^ stream);

					static MouseButtonCommand^ Parse(array<uint8_t>^ buffer, int offset);

					static MouseButtonCommand^ Parse(System::IO::Stream^ stream);

				private:
					MouseButton m_button;
					MouseButtonEvent m_buttonEvent;
				};
			}
		}
	}
}

#include "MouseButtonCommand.inl"