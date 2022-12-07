#include "pch.h"

#include "Manipulation/CommandCode.hpp"
#include "Manipulation/CommandStreamParser.hpp"
#include "Manipulation/Commands.hpp"

using namespace System;
using namespace System::IO;
using namespace RemoteControl::IO::Mouse::Manipulation;
using namespace RemoteControl::IO::Keyboard::Manipulation;

namespace RemoteControl 
{
	namespace IO 
	{
		namespace Manipulation 
		{
			CommandStreamParser::CommandStreamParser(Stream^ stream) :
				m_stream(stream)
			{
			}

			ICommand^ CommandStreamParser::ParseNext() 
			{
				const uint32_t CMD_START = 0x434D44;

				if (m_stream == nullptr) 
				{
					return nullptr;
				}

				uint8_t lastByte = 0;
				
				do
				{
					lastByte = m_stream->ReadByte();
				} 
				while (lastByte == 0);

				uint32_t cmdStart = lastByte;
				cmdStart |= m_stream->ReadByte() << 8;
				cmdStart |= m_stream->ReadByte() << 16;

				while (cmdStart != CMD_START)
				{
					cmdStart >>= 8;
					cmdStart = m_stream->ReadByte() << 16;
				}

				CommandCode commandCode = (CommandCode)m_stream->ReadByte();
				ICommand^ command = nullptr;

				switch (commandCode)
				{
				case CommandCode::MouseButton:
					command = MouseButtonCommand::Parse(m_stream);
					break;
				case CommandCode::MouseMove:
					command = MouseMoveCommand::Parse(m_stream);
					break;
				case CommandCode::MouseScroll:
					command = MouseScrollCommand::Parse(m_stream);
					break;
				case CommandCode::KeyboardKey:
					command = KeyboardKeyCommand::Parse(m_stream);
					break;
				default:
					throw gcnew FormatException("Invalid stream format");
				}

				return command;
			}
		}
	}
}