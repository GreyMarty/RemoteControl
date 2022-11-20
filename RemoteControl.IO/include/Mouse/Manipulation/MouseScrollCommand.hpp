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
				public ref class MouseScrollCommand : public IO::Manipulation::ICommand
				{
				public:
					property int Delta
					{
						int get();
						void set(int);
					}


					virtual void Execute();

				private:
					int m_delta;
				};
			}
		}
	}
}

#include "MouseScrollCommand.inl"