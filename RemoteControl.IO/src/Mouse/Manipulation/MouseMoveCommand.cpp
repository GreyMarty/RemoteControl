#include "pch.h"

#include "Mouse/Manipulation/MouseMoveCommand.hpp"

using namespace RemoteControl::Core;

namespace RemoteControl 
{
	namespace IO 
	{
		namespace Mouse
		{
			namespace Manipulation
			{
				void MouseMoveCommand::Execute()
				{
					Mouse::Move(m_delta);
				}
			}
		}
	}
}