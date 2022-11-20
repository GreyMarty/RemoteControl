#include "pch.h"

#include <windows.h>
#include <winuser.h>
#include "Mouse/Mouse.hpp"

using namespace RemoteControl::Core;

namespace RemoteControl
{
	namespace IO
	{
		namespace Mouse
		{
			static DWORD ButtonToDwEventFlags(MouseButton button, MouseButtonEvent event) 
			{
				DWORD flags = 0x0000;

				DWORD mouseButtonUpFlags[] = { MOUSEEVENTF_LEFTUP, MOUSEEVENTF_MIDDLEUP, MOUSEEVENTF_RIGHTUP };
				DWORD mouseButtonDownFlags[] = { MOUSEEVENTF_LEFTDOWN, MOUSEEVENTF_MIDDLEDOWN, MOUSEEVENTF_RIGHTDOWN };

				MouseButton mouseButtons[] = { MouseButton::Left, MouseButton::Middle, MouseButton::Right, MouseButton::X };

				if ((bool)(event & MouseButtonEvent::Up))
				{
					for (int i = 0; i < 3; i++)
					{
						flags |= mouseButtonUpFlags[i] * ((DWORD)(button & mouseButtons[i]) != 0);
					}
				}

				if ((bool)(event & MouseButtonEvent::Down))
				{
					for (int i = 0; i < 3; i++)
					{
						flags |= mouseButtonDownFlags[i] * ((DWORD)(button & mouseButtons[i]) != 0);
					}
				}

				return flags;
			}

			PointI Mouse::GetPosition()
			{
				POINT point;
				GetCursorPos(&point);

				return PointI(point.x, point.y);
			}

			void Mouse::SetPosition(PointI position)
			{
				SetCursorPos(position.X, position.Y);
			}

			void Mouse::Move(PointI delta)
			{
				INPUT input;
				ZeroMemory(&input, sizeof(INPUT));

				input.type = INPUT_MOUSE;

				input.mi.dx = delta.X;
				input.mi.dy = delta.Y;

				input.mi.dwFlags = MOUSEEVENTF_MOVE;

				SendInput(1, &input, sizeof(INPUT));
			}

			void Mouse::ButtonDown(MouseButton button)
			{
				INPUT input;
				ZeroMemory(&input, sizeof(INPUT));

				input.type = INPUT_MOUSE;

				input.mi.dwFlags = ButtonToDwEventFlags(button, MouseButtonEvent::Down);

				SendInput(1, &input, sizeof(INPUT));
			}

			void Mouse::ButtonUp(MouseButton button)
			{
				INPUT input;
				ZeroMemory(&input, sizeof(INPUT));

				input.type = INPUT_MOUSE;

				input.mi.dwFlags = ButtonToDwEventFlags(button, MouseButtonEvent::Up);

				SendInput(1, &input, sizeof(INPUT));
			}

			void Mouse::Click(MouseButton button)
			{
				INPUT input;
				ZeroMemory(&input, sizeof(INPUT));

				input.type = INPUT_MOUSE;

				input.mi.dwFlags = ButtonToDwEventFlags(button, MouseButtonEvent::Up | MouseButtonEvent::Down);

				SendInput(1, &input, sizeof(INPUT));
			}

			void Mouse::Scroll(int delta)
			{
				INPUT input;
				ZeroMemory(&input, sizeof(INPUT));

				input.type = INPUT_MOUSE;

				input.mi.dwFlags = MOUSEEVENTF_WHEEL;
				input.mi.mouseData = delta;

				SendInput(1, &input, sizeof(INPUT));
			}
		}
	}
}
