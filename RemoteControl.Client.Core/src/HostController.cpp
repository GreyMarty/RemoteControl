#include "pch.h"

#include <iostream>
#include <unordered_map>
#include "SDL2/SDL.h"
#include "HostController.hpp"
#include "SDLInitializer.hpp"

using namespace System;
using namespace System::IO;
using namespace System::Threading;
using namespace RemoteControl::Core;
using namespace RemoteControl::IO::Keyboard;
using namespace RemoteControl::IO::Keyboard::Manipulation;
using namespace RemoteControl::IO::Mouse;
using namespace RemoteControl::IO::Mouse::Manipulation;

namespace RemoteControl 
{
	namespace Client 
	{
		namespace Core 
		{
			static uint32_t SDLK2KeyboardKey(uint32_t code)
			{
				static std::unordered_map<uint32_t, uint32_t> map = {
					{ 0x00000008,	(uint32_t)KeyboardKey::Backspace   },
					{ 0x00000009,	(uint32_t)KeyboardKey::Tab         },
					{ 0x4000009C,	(uint32_t)KeyboardKey::Clear       },
					{ 0x0000000D,	(uint32_t)KeyboardKey::Enter       },
					{ 0x400000E1,	(uint32_t)KeyboardKey::Shift       },
					{ 0x400000E0,	(uint32_t)KeyboardKey::Ctrl        },
					{ 0x400000E2,	(uint32_t)KeyboardKey::Alt         },
					{ 0x40000048,	(uint32_t)KeyboardKey::Pause       },
					{ 0x40000039,	(uint32_t)KeyboardKey::CapsLock    },
					{ 0x0000001B,	(uint32_t)KeyboardKey::Esc         },
					{ 0x00000020,	(uint32_t)KeyboardKey::Spacebar    },
					{ 0x4000004B,	(uint32_t)KeyboardKey::PageUp      },
					{ 0x4000004E,	(uint32_t)KeyboardKey::PageDown    },
					{ 0x4000004D,	(uint32_t)KeyboardKey::End         },
					{ 0x4000004A,	(uint32_t)KeyboardKey::Home        },
					{ 0x40000050,	(uint32_t)KeyboardKey::ArrowLeft   },
					{ 0x40000052,	(uint32_t)KeyboardKey::ArrowUp     },
					{ 0x4000004F,	(uint32_t)KeyboardKey::ArrowRight  },
					{ 0x40000051,	(uint32_t)KeyboardKey::ArrowDown   },
					{ 0x40000077,	(uint32_t)KeyboardKey::Select      },
					{ 0x40000046,	(uint32_t)KeyboardKey::PrintScreen },
					{ 0x40000049,	(uint32_t)KeyboardKey::Ins         },
					{ 0x0000007F,	(uint32_t)KeyboardKey::Del         },
					{ 0x00000030,   (uint32_t)KeyboardKey::Num0        },
					{ 0x00000031,   (uint32_t)KeyboardKey::Num1        },
					{ 0x00000032,   (uint32_t)KeyboardKey::Num2        },
					{ 0x00000033,   (uint32_t)KeyboardKey::Num3        },
					{ 0x00000034,   (uint32_t)KeyboardKey::Num4        },
					{ 0x00000035,   (uint32_t)KeyboardKey::Num5        },
					{ 0x00000036,   (uint32_t)KeyboardKey::Num6        },
					{ 0x00000037,   (uint32_t)KeyboardKey::Num7        },
					{ 0x00000038,   (uint32_t)KeyboardKey::Num8        },
					{ 0x00000039,   (uint32_t)KeyboardKey::Num9        },
					{ 0x00000061,   (uint32_t)KeyboardKey::A           },
					{ 0x00000062,   (uint32_t)KeyboardKey::B           },
					{ 0x00000063,   (uint32_t)KeyboardKey::C           },
					{ 0x00000064,   (uint32_t)KeyboardKey::D           },
					{ 0x00000065,   (uint32_t)KeyboardKey::E           },
					{ 0x00000066,   (uint32_t)KeyboardKey::F           },
					{ 0x00000067,   (uint32_t)KeyboardKey::G           },
					{ 0x00000068,   (uint32_t)KeyboardKey::H           },
					{ 0x00000069,   (uint32_t)KeyboardKey::I           },
					{ 0x0000006A,   (uint32_t)KeyboardKey::J           },
					{ 0x0000006B,   (uint32_t)KeyboardKey::K           },
					{ 0x0000006C,   (uint32_t)KeyboardKey::L           },
					{ 0x0000006D,   (uint32_t)KeyboardKey::M           },
					{ 0x0000006E,   (uint32_t)KeyboardKey::N           },
					{ 0x0000006F,   (uint32_t)KeyboardKey::O           },
					{ 0x00000070,   (uint32_t)KeyboardKey::P           },
					{ 0x00000071,   (uint32_t)KeyboardKey::Q           },
					{ 0x00000072,   (uint32_t)KeyboardKey::R           },
					{ 0x00000073,   (uint32_t)KeyboardKey::S           },
					{ 0x00000074,   (uint32_t)KeyboardKey::T           },
					{ 0x00000075,   (uint32_t)KeyboardKey::U           },
					{ 0x00000076,   (uint32_t)KeyboardKey::V           },
					{ 0x00000077,   (uint32_t)KeyboardKey::W           },
					{ 0x00000078,   (uint32_t)KeyboardKey::X           },
					{ 0x00000079,   (uint32_t)KeyboardKey::Y           },
					{ 0x0000007A,   (uint32_t)KeyboardKey::Z           },
					{ 0x40000062,   (uint32_t)KeyboardKey::NumPad0     },
					{ 0x40000059,   (uint32_t)KeyboardKey::NumPad1     },
					{ 0x4000005A,   (uint32_t)KeyboardKey::NumPad2     },
					{ 0x4000005B,   (uint32_t)KeyboardKey::NumPad3     },
					{ 0x4000005C,   (uint32_t)KeyboardKey::NumPad4     },
					{ 0x4000005D,   (uint32_t)KeyboardKey::NumPad5     },
					{ 0x4000005E,   (uint32_t)KeyboardKey::NumPad6     },
					{ 0x4000005F,   (uint32_t)KeyboardKey::NumPad7     },
					{ 0x40000060,   (uint32_t)KeyboardKey::NumPad8     },
					{ 0x40000061,   (uint32_t)KeyboardKey::NumPad9     },
					{ 0x40000055,   (uint32_t)KeyboardKey::Mul         },
					{ 0x40000057,   (uint32_t)KeyboardKey::Add         },
					{ 0x40000056,   (uint32_t)KeyboardKey::Sub         },
					{ 0x40000063,   (uint32_t)KeyboardKey::Decimal     },
					{ 0x40000054,   (uint32_t)KeyboardKey::Div         },
					{ 0x4000003A,   (uint32_t)KeyboardKey::F1          },
					{ 0x4000003B,   (uint32_t)KeyboardKey::F2          },
					{ 0x4000003C,   (uint32_t)KeyboardKey::F3          },
					{ 0x4000003D,   (uint32_t)KeyboardKey::F4          },
					{ 0x4000003E,   (uint32_t)KeyboardKey::F5          },
					{ 0x4000003F,   (uint32_t)KeyboardKey::F6          },
					{ 0x40000040,   (uint32_t)KeyboardKey::F7          },
					{ 0x40000041,   (uint32_t)KeyboardKey::F8          },
					{ 0x40000042,   (uint32_t)KeyboardKey::F9          },
					{ 0x40000043,   (uint32_t)KeyboardKey::F10         },
					{ 0x40000044,   (uint32_t)KeyboardKey::F11         },
					{ 0x40000045,   (uint32_t)KeyboardKey::F12         },
					{ 0x40000068,   (uint32_t)KeyboardKey::F13         },
					{ 0x40000069,   (uint32_t)KeyboardKey::F14         },
					{ 0x4000006A,   (uint32_t)KeyboardKey::F15         },
					{ 0x4000006B,   (uint32_t)KeyboardKey::F16         },
					{ 0x4000006C,   (uint32_t)KeyboardKey::F17         },
					{ 0x4000006D,   (uint32_t)KeyboardKey::F18         },
					{ 0x4000006E,   (uint32_t)KeyboardKey::F19         },
					{ 0x4000006F,   (uint32_t)KeyboardKey::F20         },
					{ 0x40000070,   (uint32_t)KeyboardKey::F21         },
					{ 0x40000071,   (uint32_t)KeyboardKey::F22         },
					{ 0x40000072,   (uint32_t)KeyboardKey::F23         },
					{ 0x40000073,   (uint32_t)KeyboardKey::F24         },
					{ 0x400000E1,   (uint32_t)KeyboardKey::LShift      },
					{ 0x400000E5,   (uint32_t)KeyboardKey::RShift      },
					{ 0x400000E0,   (uint32_t)KeyboardKey::LCtrl       },
					{ 0x400000E4,   (uint32_t)KeyboardKey::RCtrl       },
					{ 0x400000E2,   (uint32_t)KeyboardKey::LAlt        },
					{ 0x400000E6,   (uint32_t)KeyboardKey::RAlt        }
				};

				if (map.find(code) == map.end()) 
				{
					return 0;
				}

				return map[code];
			}

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

				m_width = displayMode.w;
				m_height = displayMode.h;

				m_window = SDL_CreateWindow(
					"Remote Desktop",
					SDL_WINDOWPOS_CENTERED,
					SDL_WINDOWPOS_CENTERED,
					displayMode.w,
					displayMode.h,
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
				m_decoderThread->Join();
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

				CleanUp();
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
				std::cout << "Key" << (pressed ? "Down" : "Up") << "(" << SDLK2KeyboardKey(event.key.keysym.sym) << ")\n";

				KeyboardKeyCommand^ command = gcnew KeyboardKeyCommand();
				command->KeyEvent = pressed ? KeyboardKeyEvent::Press : KeyboardKeyEvent::Release;
				command->Scancode = (uint32_t)SDLK2KeyboardKey(event.key.keysym.sym);

				command->WriteTo(m_commandStream);

				if (event.key.keysym.sym == SDLK_ESCAPE) 
				{
					Close();
				}
			}

			void HostController::ProcessMouseButtonEvent(const SDL_Event& event, bool pressed) 
			{
				std::cout << "MouseButton " << (pressed ? "Down" : "Up") << "(" << event.button.button << ")\n";

				MouseButtonCommand^ command = gcnew MouseButtonCommand();
				
				switch (event.button.button)
				{
				case SDL_BUTTON_LEFT:
					command->Button = MouseButton::Left;
					break;
				case SDL_BUTTON_RIGHT:
					command->Button = MouseButton::Right;
					break;
				case SDL_BUTTON_MIDDLE:
					command->Button = MouseButton::Middle;
					break;
				}

				command->ButtonEvent = pressed ? MouseButtonEvent::Down : MouseButtonEvent::Up;

				command->WriteTo(m_commandStream);
			}

			void HostController::ProcessMouseMoveEvent(const SDL_Event& event) 
			{
				std::cout << "MouseMoved " << "x: " << event.motion.x << ", y: " << event.motion.y << "\n";

				MouseMoveCommand^ command = gcnew MouseMoveCommand();
				command->Delta = RemoteControl::Core::PointI(
					event.motion.x,
					event.motion.y
				);
				command->IsAbsolute = true;

				command->WriteTo(m_commandStream);
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

					while (!bitmap && m_running)
					{
						bitmap = m_videoStream->NextFrame();
					}

					if (!bitmap) 
					{
						continue;
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

			void HostController::CleanUp() 
			{
				SDL_DestroyWindow(m_window);
				SDL_DestroyRenderer(m_renderer);
				SDL_DestroyMutex(m_drawMutex);
			}
		}
	}
}