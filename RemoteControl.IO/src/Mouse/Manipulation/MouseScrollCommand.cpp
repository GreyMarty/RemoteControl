#include "pch.h"

#include "Mouse/Manipulation/MouseScrollCommand.hpp"
#include "Manipulation/CommandCode.hpp"

using namespace System;
using namespace System::IO;
using namespace System::Runtime::InteropServices;
using namespace RemoteControl::IO::Manipulation;

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

				MouseScrollCommand^ MouseScrollCommand::Parse(array<uint8_t>^ buffer, int offset) 
				{
					MouseScrollCommand^ command = gcnew MouseScrollCommand();

					int delta;

					Marshal::Copy(buffer, offset, static_cast<IntPtr>(&delta), sizeof(int));

					command->m_delta = delta;

					return command;
				}

				MouseScrollCommand^ MouseScrollCommand::Parse(Stream^ stream)
				{
					int targetBytes = sizeof(int);
					int nbytes = 0;

					int delta;

					array<uint8_t>^ buffer = gcnew array<uint8_t>(sizeof(int));
					
					nbytes += stream->Read(buffer);
					Marshal::Copy(buffer, 0, static_cast<IntPtr>(&delta), sizeof(int));

					if (nbytes < targetBytes) 
					{
						stream->Position -= nbytes;
						return nullptr;
					}

					MouseScrollCommand^ command = gcnew MouseScrollCommand();
					command->m_delta = delta;

					return command;
				}

				int MouseScrollCommand::WriteTo(Stream^ stream) 
				{
					stream->WriteByte((uint8_t)CommandCode::MouseScroll);
					stream->Write(BitConverter::GetBytes(m_delta));

					return sizeof(int) + 1;
				}
			}
		}
	}
}