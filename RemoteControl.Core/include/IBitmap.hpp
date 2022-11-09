#pragma once

#include "inttypes.h"

namespace RemoteControl 
{
	namespace Core 
	{
		public interface class IBitmap 
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

			property uint32_t Stride 
			{
				uint32_t get();
			}

			property uint16_t BitsPerPixel 
			{
				uint16_t get();
			}

			property uint64_t SizeBytes 
			{
				uint64_t get();
			}

			property System::IntPtr Scan0 
			{
				System::IntPtr get();
			}

			void LockBits();

			void UnlockBits();
		};
	}
}