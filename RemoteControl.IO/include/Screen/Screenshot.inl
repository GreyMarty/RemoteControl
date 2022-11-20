namespace RemoteControl
{
	namespace IO
	{
		namespace Screen
		{
			inline uint32_t Screenshot::Width::get()
			{
				return m_width;
			}

			inline uint32_t Screenshot::Height::get()
			{
				return m_height;
			}

			inline uint16_t Screenshot::BitsPerPixel::get()
			{
				return m_bitsPerPixel;
			}

			inline uint32_t Screenshot::Stride::get()
			{
				return m_stride;
			}

			inline uint64_t Screenshot::SizeBytes::get()
			{
				return m_sizeBytes;
			}

			inline System::IntPtr Screenshot::Scan0::get()
			{
				return static_cast<System::IntPtr>(m_scan0);
			}
		}
	}
}
