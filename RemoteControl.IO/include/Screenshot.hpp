#pragma once

#include "inttypes.h"
#include "windows.h"

namespace RemoteControl
{
	namespace Core
	{
		public ref class Screenshot sealed : IBitmap
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

			virtual property uint16_t BitsPerPixel
			{
				uint16_t get();
			}

			virtual property uint32_t Stride
			{
				uint32_t get();
			}

			virtual property uint64_t SizeBytes
			{
				uint64_t get();
			}

			virtual property System::IntPtr Scan0
			{
				System::IntPtr get();
			}

			Screenshot(const HBITMAP hBitmap, const HDC hDc);

			Screenshot(System::IntPtr hBitmap, System::IntPtr hDc);

			~Screenshot();

			!Screenshot();

			virtual void LockBits();

			virtual void UnlockBits();

		private:
			const HDC m_hDc;
			const HBITMAP m_hBitmap;

			uint32_t m_width;
			uint32_t m_height;

			uint32_t m_stride;
			uint16_t m_bitsPerPixel;

			uint64_t m_sizeBytes;

			void* m_scan0;
		};
	}
}

#include "Screenshot.inl"