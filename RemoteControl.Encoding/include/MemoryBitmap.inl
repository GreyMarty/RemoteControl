namespace RemoteControl 
{
	namespace Encoding 
	{
		inline uint32_t MemoryBitmap::Width::get()
		{
			return m_width;
		}

		inline uint32_t MemoryBitmap::Height::get()
		{
			return m_height;
		}

		inline uint16_t MemoryBitmap::BitsPerPixel::get()
		{
			return m_bitsPerPixel;
		}

		inline uint32_t MemoryBitmap::Stride::get()
		{
			return m_stride;
		}

		inline uint64_t MemoryBitmap::SizeBytes::get()
		{
			return m_sizeBytes;
		}

		inline System::IntPtr MemoryBitmap::Scan0::get()
		{
			return static_cast<System::IntPtr>(m_scan0);
		}
	}
}
