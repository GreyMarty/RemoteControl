#pragma once
#include "inttypes.h"
#include "IBitmap.hpp"

namespace RemoteControl
{
	namespace Core
	{
		public interface class IVideoStream
		{
		public:
			property uint32_t Width 
			{
				uint32_t get();
			}

			property uint32_t Height
			{
				uint32_t get();
			}

			IBitmap^ NextFrame();
		};
	}
}