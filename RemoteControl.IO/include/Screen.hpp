#pragma once

namespace RemoteControl 
{
	namespace IO 
	{
		public ref class Screen abstract sealed 
		{
		public:
			static Core::Screenshot^ Capture();
		};
	}
}