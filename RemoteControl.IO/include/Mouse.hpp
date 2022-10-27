#pragma once

#include "MouseEvent.hpp"
#include "MouseButton.hpp"

namespace RemoteControl 
{
	namespace IO
	{
		public ref class Mouse abstract sealed 
		{
		public:
			static Core::PointU GetPosition();

			static void SetPosition(Core::PointU position);
			
			static void Move(Core::PointI delta);

			static void ButtonDown(MouseButton button);

			static void ButtonUp(MouseButton button);

			static void Click(MouseButton button);

			static void Scroll(int delta);
			
		private:
			static DWORD ButtonToDwEventFlags(MouseButton button, MouseButtonEvent event);
		};
	}
}