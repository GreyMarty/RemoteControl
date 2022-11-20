#pragma once

namespace RemoteControl
{
	namespace IO
	{
		namespace Screen
		{
			public ref class WindowsScreenRecorder sealed : public Core::IVideoStream
			{
			public:
				virtual property uint32_t Width
				{
					uint32_t get();
				}

				virtual property uint32_t Height
				{
					uint32_t get();
				}

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
}