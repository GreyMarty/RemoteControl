#include "pch.h"

#include "SDL2/SDL.h"
#include "SDLInitializer.hpp"

using namespace System;

namespace RemoteControl 
{
	namespace Client 
	{
		namespace Core 
		{
			bool SDLInitializer::m_initialized = false;

			void SDLInitializer::Init() 
			{
				if (m_initialized) 
				{
					return;
				}

				int ret = SDL_Init(SDL_INIT_VIDEO);
				if (ret < 0) 
				{
					gcnew Exception("Failed to initialize the SDL2 library!");
				}
			}
		}
	}
}