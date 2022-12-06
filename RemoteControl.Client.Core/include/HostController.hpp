#pragma once

namespace RemoteControl 
{
	namespace Client 
	{
		namespace Core 
		{
			public ref class HostController 
			{
			public:
				HostController(RemoteControl::Core::IVideoStream^ videoStream, System::IO::Stream^ commandStream);

				~HostController();

				!HostController();

				void Show();

				void Close();

			private:
				RemoteControl::Core::IVideoStream^ m_videoStream;
				System::IO::Stream^ m_commandStream;

				SDL_Window* m_window;
				SDL_Renderer* m_renderer;

				SDL_mutex* m_drawMutex;

				bool m_running;

				int m_width;
				int m_height;

				RemoteControl::Core::IBitmap^ m_screen;

				System::Threading::Thread^ m_decoderThread;

				void Mainloop();

				void ProcessEvent(const SDL_Event& event);

				void ProcessKeyEvent(const SDL_Event& event, bool pressed);

				void ProcessMouseButtonEvent(const SDL_Event& event, bool pressed);

				void ProcessMouseMoveEvent(const SDL_Event& event);

				void DrawBitmap();

				void Decode();

				void CleanUp();
			};
		}
	}
}