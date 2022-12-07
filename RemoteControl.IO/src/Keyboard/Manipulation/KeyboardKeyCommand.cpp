#include "pch.h"

#include <windows.h>

#include "Keyboard/Keyboard.hpp"
#include "Keyboard/Manipulation/KeyboardKeyCommand.hpp"
#include "Manipulation/CommandCode.hpp"

using namespace System;
using namespace System::IO;
using namespace System::Runtime::InteropServices;
using namespace RemoteControl::IO::Manipulation;

namespace RemoteControl
{
	namespace IO 
	{
		namespace Keyboard 
		{
			namespace Manipulation 
			{
				void KeyboardKeyCommand::Execute() 
				{
					if (m_keyEvent.HasFlag(KeyboardKeyEvent::Press) && m_keyEvent.HasFlag(KeyboardKeyEvent::Release)) 
					{
						Keyboard::ClickKey((KeyboardKey)m_scanCode);
					}
					else if (m_keyEvent.HasFlag(KeyboardKeyEvent::Press)) 
					{
						Keyboard::PressKey((KeyboardKey)m_scanCode);
					}
					else if (m_keyEvent.HasFlag(KeyboardKeyEvent::Release)) 
					{
						Keyboard::ReleaseKey((KeyboardKey)m_scanCode);
					}
				}

				int KeyboardKeyCommand::WriteTo(Stream^ stream) 
				{
					stream->WriteByte((uint8_t)CommandCode::KeyboardKey);
					stream->Write(BitConverter::GetBytes(m_scanCode));
					stream->WriteByte((uint8_t)m_keyEvent);

					return sizeof(uint8_t) + sizeof(uint32_t) + 1;
				}

				KeyboardKeyCommand^ KeyboardKeyCommand::Parse(Stream^ stream)
				{
					const int targetBytes = sizeof(uint8_t) + sizeof(uint32_t);
					int nbytes = 0;

					uint32_t scanCode;
					uint8_t keyEvent;

					array<uint8_t>^ buffer = gcnew array<uint8_t>(sizeof(int));

					nbytes += stream->Read(buffer);
					Marshal::Copy(buffer, 0, static_cast<IntPtr>(&scanCode), sizeof(uint32_t));

					nbytes += stream->Read(buffer);
					Marshal::Copy(buffer, 0, static_cast<IntPtr>(&keyEvent), sizeof(uint8_t));

					if (nbytes < targetBytes) 
					{
						stream->Position -= nbytes;
						return nullptr;
					}

					KeyboardKeyCommand^ command = gcnew KeyboardKeyCommand();
					command->Scancode = scanCode;
					command->KeyEvent = (KeyboardKeyEvent)keyEvent;

					return command;
				}
			}
		}
	}
}