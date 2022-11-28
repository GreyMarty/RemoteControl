#include "pch.h"

#include <iostream>
#include "SDL2/SDL.h"
#include "HostController.hpp"
#include "SDLInitializer.hpp"

using namespace System;
using namespace System::IO;
using namespace System::Threading;
using namespace RemoteControl::Core;

namespace RemoteControl 
{
	namespace Client 
	{
		namespace Core 
		{
			HostController::HostController(IVideoStream^ videoStream, Stream^ commandStream) :
				m_videoStream(videoStream),
				m_commandStream(commandStream)
			{
			}

			HostController::~HostController() 
			{
				Close();
			}

			HostController::!HostController() 
			{
				this->~HostController();
			}

			void HostController::Show() 
			{
				if (m_window) 
				{
					return;
				}

				SDLInitializer::Init();

				SDL_DisplayMode displayMode;
				SDL_GetCurrentDisplayMode(0, &displayMode);

				m_window = SDL_CreateWindow(
					"Remote Desktop",
					SDL_WINDOWPOS_CENTERED,
					SDL_WINDOWPOS_CENTERED,
					displayMode.w - 100,
					displayMode.h - 100,
					SDL_WINDOW_BORDERLESS
				);

				if (!m_window) 
				{
					throw gcnew Exception("Failed to create window");
				}

				m_renderer = SDL_CreateRenderer(m_window, -1, 0);

				if (!m_renderer) 
				{
					throw gcnew Exception("Failed to get window surface");
				}

				m_running = true;

				m_drawMutex = SDL_CreateMutex();

				m_decoderThread = gcnew Thread(gcnew ThreadStart(this, &HostController::Decode));
				m_decoderThread->Start();

				Mainloop();
			}

			void HostController::Close() 
			{
				m_running = false;
			}

			void HostController::Mainloop() 
			{
				SDL_Event e;

				while (m_running) 
				{
					while (SDL_PollEvent(&e))
					{
						ProcessEvent(e);
					}

					SDL_RenderClear(m_renderer);
					DrawBitmap();
					SDL_RenderPresent(m_renderer);
				}

				SDL_DestroyWindow(m_window);
				SDL_DestroyMutex(m_drawMutex);
			}

			void HostController::ProcessEvent(const SDL_Event& event) 
			{
				switch (event.type)
				{
				case SDL_QUIT:
					m_running = false;
					break;

				case SDL_KEYDOWN:
					ProcessKeyEvent(event, true);
					break;

				case SDL_KEYUP:
					ProcessKeyEvent(event, false);
					break;

				case SDL_MOUSEBUTTONDOWN:
					ProcessMouseButtonEvent(event, true);
					break;

				case SDL_MOUSEBUTTONUP:
					ProcessMouseButtonEvent(event, false);
					break;

				case SDL_MOUSEMOTION:
					ProcessMouseMoveEvent(event);
					break;

				default:
					break;
				}
			}

			void HostController::ProcessKeyEvent(const SDL_Event& event, bool pressed) 
			{
				std::cout << "Key" << (pressed ? "Down" : "Up") << "(" << event.key.keysym.scancode << ")\n";

				if (event.key.keysym.sym == SDLK_ESCAPE) 
				{
					Close();
				}
			}

			void HostController::ProcessMouseButtonEvent(const SDL_Event& event, bool pressed) 
			{
				std::cout << "MouseButton " << (pressed ? "Down" : "Up") << "(" << event.button.button << ")\n";
			}

			void HostController::ProcessMouseMoveEvent(const SDL_Event& event) 
			{
				std::cout << "MouseMoved " << "x: " << event.motion.x << ", y: " << event.motion.y << "\n";
			}

			void HostController::DrawBitmap() 
			{
				if (!m_screen) 
				{
					return;
				}


				SDL_LockMutex(m_drawMutex);

				SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(
					static_cast<void*>(m_screen->Scan0),
					m_screen->Width,
					m_screen->Height,
					m_screen->BitsPerPixel,
					m_screen->Stride,
					0xFF0000,
					0x00FF00,
					0x0000FF,
					0
				);

				SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surface);

				SDL_RenderCopy(m_renderer, texture, NULL, NULL);
				
				SDL_FreeSurface(surface);
				SDL_DestroyTexture(texture);

				SDL_UnlockMutex(m_drawMutex);
			}

			void HostController::Decode() 
			{
				while (m_running) 
				{
					IBitmap^ bitmap = nullptr;

					while (!bitmap)
					{
						bitmap = m_videoStream->NextFrame();
					}

					SDL_LockMutex(m_drawMutex);

					if (m_screen) 
					{
						m_screen->UnlockBits();
					}

					m_screen = bitmap;
					m_screen->LockBits();

					SDL_UnlockMutex(m_drawMutex);
				}
			}
		}
	}
}