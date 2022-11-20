#pragma once

#include "MouseEvent.hpp"
#include "MouseButton.hpp"

namespace RemoteControl
{
	namespace IO
	{
		namespace Mouse
		{
			public ref class Mouse abstract sealed
			{
			public:
				static Core::PointI GetPosition();

				static void SetPosition(Core::PointI position);

				static void Move(Core::PointI delta);

				static void ButtonDown(MouseButton button);

				static void ButtonUp(MouseButton button);

				static void Click(MouseButton button);

				static void Scroll(int delta);
			};
		}
	}
}