namespace RemoteControl 
{
	namespace Core 
	{
		inline unsigned long Bitmap::Width::get() 
		{
			return m_width;
		}

		inline unsigned long Bitmap::Height::get()
		{
			return m_height;
		}

		inline unsigned long Bitmap::ScanWidth::get()
		{
			return m_scanWidth;
		}

		inline unsigned short Bitmap::Planes::get()
		{
			return m_planes;
		}

		inline unsigned short Bitmap::BitsPerPixel::get()
		{
			return m_bitsPerPixel;
		}

		inline unsigned long Bitmap::SizeBytes::get() 
		{
			return ((m_bitsPerPixel * m_width + 31) / 32) * 4 * m_height;
		}

		inline System::IntPtr Bitmap::Scan0::get() 
		{
			return static_cast<System::IntPtr>(m_scan0);
		}
	}
}
