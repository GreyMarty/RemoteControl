#pragma once

namespace RemoteControl 
{
	namespace IO 
	{
		namespace Manipulation 
		{
			public enum class CommandCode 
			{
				Nop			= 0x00,
				MouseButton	= 0x01,
				MouseMove	= 0x02,
				MouseScroll	= 0x03,
				KeyboardKey	= 0x04,
			};
		}
	}
}