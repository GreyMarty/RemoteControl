#pragma once

#include "windows.h"

namespace RemoteControl 
{
	namespace IO 
	{
		public ref class WindowsScreenRecorder sealed : public Core::IVideoStream
		{
		public:
			WindowsScreenRecorder();

			virtual Core::IBitmap^ NextFrame();

		private:
			HDC m_hScreenDc;
			HDC m_hMemoryDc;

			int m_screenWidth;
			int m_screenHeight;
		};
	}
}