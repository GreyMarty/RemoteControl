#pragma once

namespace RemoteControl
{
	namespace Core
	{
		namespace Mouse
		{
			public enum class MouseEvent
			{
				Absolute = 0x8000,
				Move = 0x0001,
				LeftDown = 0x0002,
				LeftUp = 0x0004,
				MiddleDown = 0x0020,
				MiddleUp = 0x0040,
				RightDown = 0x0008,
				RightUp = 0x0010,
				Wheel = 0x0800,
				HWheel = 0x0100,
				XDown = 0x0080,
				XUp = 0x0100
			};
		}
	}
}