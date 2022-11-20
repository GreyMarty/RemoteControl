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
						Keyboard::ClickKey(m_key);
					}
					else if (m_keyEvent.HasFlag(KeyboardKeyEvent::Press)) 
					{
						Keyboard::PressKey(m_key);
					}
					else if (m_keyEvent.HasFlag(KeyboardKeyEvent::Release)) 
					{
						Keyboard::ReleaseKey(m_key);
					}
				}

				int KeyboardKeyCommand::WriteTo(Stream^ stream) 
				{
					stream->WriteByte((uint8_t)CommandCode::KeyboardKey);
					stream->Write(BitConverter::GetBytes((int)m_key));
					stream->Write(BitConverter::GetBytes((int)m_keyEvent));

					return sizeof(int) * 2 + 1;
				}

				KeyboardKeyCommand^ KeyboardKeyCommand::Parse(Stream^ stream)
				{
					const int targetBytes = sizeof(int) * 2;
					int nbytes = 0;

					int key;
					int keyEvent;

					array<uint8_t>^ buffer = gcnew array<uint8_t>(sizeof(int));

					nbytes += stream->Read(buffer);
					Marshal::Copy(buffer, 0, static_cast<IntPtr>(&key), sizeof(int));

					nbytes += stream->Read(buffer);
					Marshal::Copy(buffer, 0, static_cast<IntPtr>(&keyEvent), sizeof(int));

					if (nbytes < targetBytes) 
					{
						stream->Position -= nbytes;
						return nullptr;
					}

					KeyboardKeyCommand^ command = gcnew KeyboardKeyCommand();
					command->Key = (KeyboardKey)key;
					command->KeyEvent = (KeyboardKeyEvent)keyEvent;

					return command;
				}
			}
		}
	}
}