#pragma once

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

				private:
					MouseButton m_button;
					MouseButtonEvent m_buttonEvent;
				};
			}
		}
	}
}

#include "MouseButtonCommand.inl"