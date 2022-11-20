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
				public ref class MouseMoveCommand : public IO::Manipulation::ICommand
				{
				public:
					property Core::PointI Delta
					{
						Core::PointI get();
						void set(Core::PointI delta);
					}

					virtual void Execute();

				private:
					Core::PointI m_delta;
				};
			}
		}
	}
}

#include "MouseMoveCommand.inl"