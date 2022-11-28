#pragma once

namespace RemoteControl 
{
	namespace Client 
	{
		namespace Core 
		{
			private class SDLInitializer abstract sealed 
			{
			public:
				static bool m_initialized;
				
				
				static void Init();
			};
		}
	}
}
