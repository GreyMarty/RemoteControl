#include "pch.h"

#include "Mouse/Manipulation/MouseMoveCommand.hpp"
#include "Manipulation/CommandCode.hpp"

using namespace System;
using namespace System::IO;
using namespace System::Runtime::InteropServices;
using namespace RemoteControl::Core;
using namespace RemoteControl::IO::Manipulation;

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
					if (m_isAbsolute) 
					{
						Mouse::SetPosition(m_delta);
					}
					else
					{
						Mouse::Move(m_delta);
					}
				}

				MouseMoveCommand^ MouseMoveCommand::Parse(array<uint8_t>^ buffer, int offset) 
				{
					MouseMoveCommand^ command = gcnew MouseMoveCommand();

					PointI delta;
					bool isAbsolute;

					Marshal::Copy(buffer, offset, static_cast<IntPtr>(&delta), sizeof(PointI));
					offset += sizeof(PointI);

					Marshal::Copy(buffer, offset, static_cast<IntPtr>(&isAbsolute), sizeof(bool));

					command->m_delta = delta;
					command->m_isAbsolute = isAbsolute;

					return command;
				}

				MouseMoveCommand^ MouseMoveCommand::Parse(Stream^ stream)
				{
					const int targetBytes = sizeof(PointI) + sizeof(bool);
					int nbytes = 0;

					PointI delta;
					bool isAbsolute;

					array<uint8_t>^ buffer = gcnew array<uint8_t>(sizeof(PointI));
					
					nbytes += stream->Read(buffer);
					Marshal::Copy(buffer, 0, static_cast<IntPtr>(&delta), sizeof(PointI));

					nbytes += stream->Read(buffer, 0, sizeof(bool));
					Marshal::Copy(buffer, 0, static_cast<IntPtr>(&isAbsolute), sizeof(bool));
					
					if (nbytes < targetBytes) 
					{
						stream->Position -= nbytes;
						return nullptr;
					}

					MouseMoveCommand^ command = gcnew MouseMoveCommand();
					command->m_delta = delta;
					command->m_isAbsolute = isAbsolute;

					return command;
				}

				int MouseMoveCommand::WriteTo(Stream^ stream) 
				{
					array<uint8_t>^ buffer = gcnew array<uint8_t>(sizeof(PointI));

					PointI delta = m_delta;
					Marshal::Copy(static_cast<IntPtr>(&delta), buffer, 0, sizeof(PointI));

					stream->WriteByte((uint8_t)CommandCode::MouseMove);
					stream->Write(buffer);
					stream->Write(BitConverter::GetBytes(m_isAbsolute));

					stream->Write(gcnew array<uint8_t> { 0, 0, 0, 0, 0, 0, 0, 0 });

					return sizeof(PointI) + sizeof(m_isAbsolute) + 1;
				}
			}
		}
	}
}