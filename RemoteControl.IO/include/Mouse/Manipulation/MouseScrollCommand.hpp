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
				public ref class MouseScrollCommand : public IO::Manipulation::ICommand
				{
				public:
					virtual property int DataSize 
					{
						int get();
					}

					property int Delta
					{
						int get();
						void set(int);
					}


					virtual void Execute();

					virtual int WriteTo(System::IO::Stream^ stream);

					static MouseScrollCommand^ Parse(array<uint8_t>^ buffer, int offset);

					static MouseScrollCommand^ Parse(System::IO::Stream^ stream);

				private:
					int m_delta;
				};
			}
		}
	}
}

#include "MouseScrollCommand.inl"