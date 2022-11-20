#include "pch.h"

#include "Mouse/Manipulation/MouseScrollCommand.hpp"

namespace RemoteControl 
{
	namespace IO
	{
		namespace Mouse
		{
			namespace Manipulation
			{
				void MouseScrollCommand::Execute()
				{
					Mouse::Scroll(m_delta);
				}
			}
		}
	}
}