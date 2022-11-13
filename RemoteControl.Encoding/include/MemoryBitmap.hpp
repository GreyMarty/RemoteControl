#pragma once

#include <inttypes.h>

namespace RemoteControl 
{
	namespace Encoding 
	{
		public ref class MemoryBitmap : public Core::IBitmap 
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

			virtual property uint32_t Stride
			{
				uint32_t get();
			}

			virtual property uint16_t BitsPerPixel
			{
				uint16_t get();
			}

			virtual property uint64_t SizeBytes
			{
				uint64_t get();
			}

			virtual property System::IntPtr Scan0
			{
				System::IntPtr get();
			}

			MemoryBitmap(uint32_t width, uint32_t height, uint32_t stride, uint16_t bitsPerPixel, System::IntPtr scan0);

			~MemoryBitmap();

			!MemoryBitmap();

			virtual void LockBits();

			virtual void UnlockBits();

		private:
			uint32_t m_width;
			uint32_t m_height;

			uint32_t m_stride;
			uint16_t m_bitsPerPixel;

			uint64_t m_sizeBytes;

			void* m_scan0;
		};
	}
}

#include "MemoryBitmap.inl"
