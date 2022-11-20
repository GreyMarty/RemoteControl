#include "pch.h"

#include "Mouse/Manipulation/MouseButtonCommand.hpp"
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
				void MouseButtonCommand::Execute()
				{
					if (m_buttonEvent.HasFlag(MouseButtonEvent::Down) && m_buttonEvent.HasFlag(MouseButtonEvent::Up))
					{
						Mouse::Mouse::Click(m_button);
					}
					else if (m_buttonEvent.HasFlag(MouseButtonEvent::Down))
					{
						Mouse::Mouse::ButtonDown(m_button);
					}
					else if (m_buttonEvent.HasFlag(MouseButtonEvent::Up))
					{
						Mouse::Mouse::ButtonUp(m_button);
					}
				}

				MouseButtonCommand^ MouseButtonCommand::Parse(array<uint8_t>^ buffer, int offset) 
				{
					MouseButtonCommand^ command = gcnew MouseButtonCommand();

					int mouseButton;
					int mouseButtonEvent;

					Marshal::Copy(buffer, offset, static_cast<IntPtr>(&mouseButton), sizeof(int));
					offset += sizeof(int);

					Marshal::Copy(buffer, offset, static_cast<IntPtr>(&mouseButtonEvent), sizeof(int));

					command->m_button = (MouseButton)mouseButton;
					command->m_buttonEvent = (MouseButtonEvent)mouseButtonEvent;

					return command;
				}

				MouseButtonCommand^ MouseButtonCommand::Parse(Stream^ stream)
				{
					const int targetBytes = sizeof(int) * 2;
					int nbytes = 0;

					int mouseButton;
					int mouseButtonEvent;

					array<uint8_t>^ buffer = gcnew array<uint8_t>(sizeof(int));

					nbytes += stream->Read(buffer);
					Marshal::Copy(buffer, 0, static_cast<IntPtr>(&mouseButton), sizeof(int));

					nbytes += stream->Read(buffer);
					Marshal::Copy(buffer, 0, static_cast<IntPtr>(&mouseButtonEvent), sizeof(int));

					if (nbytes < targetBytes) 
					{
						stream->Position -= nbytes;
						return nullptr;
					}

					MouseButtonCommand^ command = gcnew MouseButtonCommand();
					command->m_button = (MouseButton)mouseButton;
					command->m_buttonEvent = (MouseButtonEvent)mouseButtonEvent;

					return command;
				}

				int MouseButtonCommand::WriteTo(Stream^ stream) 
				{
					stream->WriteByte((uint8_t)CommandCode::MouseButton);
					stream->Write(BitConverter::GetBytes((int)m_button));
					stream->Write(BitConverter::GetBytes((int)m_buttonEvent));

					return 2 * sizeof(int) + 1;
				}
			}
		}
	}
}