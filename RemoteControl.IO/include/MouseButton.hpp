#pragma once

namespace RemoteControl
{
	namespace IO
	{
		public enum class MouseButton 
		{
			Left	= 0x0001,
			Right	= 0x0002,
			Middle	= 0x0004,
			X		= 0x0008
		};

		public enum class MouseButtonEvent 
		{
			Up		= 0x0001,
			Down	= 0x0002
		};
	}
}