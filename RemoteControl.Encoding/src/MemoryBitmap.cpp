#include "pch.h"
#include <string.h>
#include "MemoryBitmap.hpp"

namespace RemoteControl
{
	namespace Encoding
	{
		MemoryBitmap::MemoryBitmap(
			uint32_t width,
			uint32_t height,
			uint32_t stride,
			uint16_t bitsPerPixel,
			System::IntPtr scan0
		) :
			m_width(width),
			m_height(height),
			m_stride(stride),
			m_bitsPerPixel(bitsPerPixel)
		{
			m_sizeBytes = ((width * bitsPerPixel + 31) / 32) * 4 * height;
			
			m_scan0 = new uint8_t[m_sizeBytes];
			memcpy(m_scan0, (void*)scan0, m_sizeBytes);
		}

		MemoryBitmap::~MemoryBitmap() 
		{
			delete[] m_scan0;
		}

		MemoryBitmap::!MemoryBitmap() 
		{
			this->~MemoryBitmap();
		}

		void MemoryBitmap::LockBits() 
		{
		}

		void MemoryBitmap::UnlockBits() 
		{
			delete[] m_scan0;
		}
	}
}