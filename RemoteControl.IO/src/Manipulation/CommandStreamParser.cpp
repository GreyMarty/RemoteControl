#include "pch.h"

#include "Manipulation/CommandCode.hpp"
#include "Manipulation/CommandStreamParser.hpp"
#include "Manipulation/Commands.hpp"

using namespace System;
using namespace System::IO;
using namespace RemoteControl::IO::Mouse::Manipulation;

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
				CommandCode commandCode;
				
				do
				{
					commandCode = (CommandCode)m_stream->ReadByte();
				} 
				while (commandCode == CommandCode::Nop);

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
				default:
					throw gcnew FormatException("Invalid stream format");
				}

				return command;
			}
		}
	}
}