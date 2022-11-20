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
				public ref class MouseMoveCommand : public IO::Manipulation::ICommand
				{
				public:
					virtual property int DataSize 
					{
						int get();
					}

					property Core::PointI Delta
					{
						Core::PointI get();
						void set(Core::PointI delta);
					}

					property bool IsAbsolute 
					{
						bool get();
						void set(bool isAbsolute);
					}

					virtual void Execute();

					virtual int WriteTo(System::IO::Stream^ stream);

					static MouseMoveCommand^ Parse(array<uint8_t>^ buffer, int offset);

					static MouseMoveCommand^ Parse(System::IO::Stream^ stream);

				private:
					Core::PointI m_delta;
					bool m_isAbsolute;
				};
			}
		}
	}
}

#include "MouseMoveCommand.inl"