#include "pch.h"

#include "Mouse/Manipulation/MouseButtonCommand.hpp"

namespace RemoteControl 
{
	namespace IO
	{
		namespace Mouse
		{
			namespace Manipulation
			{
				void MouseButtonCommand::Execute()
				{
					if (m_buttonEvent.HasFlag(MouseButtonEvent::Down) && m_buttonEvent.HasFlag(MouseButtonEvent::Up))
					{
						Mouse::Mouse::Click(m_button);
					}
					else if (m_buttonEvent.HasFlag(MouseButtonEvent::Down))
					{
						Mouse::Mouse::ButtonDown(m_button);
					}
					else if (m_buttonEvent.HasFlag(MouseButtonEvent::Up))
					{
						Mouse::Mouse::ButtonUp(m_button);
					}
				}
			}
		}
	}
}