#pragma once

#ifndef _INC_WINDOWS
struct HBITMAP__;
using HBITMAP = HBITMAP__*;
#endif

namespace RemoteControl 
{
	namespace Core 
	{
		public ref class Bitmap sealed 
		{
		public:
			property unsigned long Width 
			{
				unsigned long get();
			}

			property unsigned long Height 
			{
				unsigned long get();
			}

			property unsigned long ScanWidth 
			{
				unsigned long get();
			}

			property unsigned short Planes 
			{
				unsigned short get();
			}

			property unsigned short BitsPerPixel 
			{
				unsigned short get();
			}

			property unsigned long SizeBytes 
			{
				unsigned long get();
			}

			property System::IntPtr Scan0 
			{
				System::IntPtr get();
			}

			Bitmap(const HBITMAP hBitmap);

			Bitmap(System::IntPtr hBitmap);

			~Bitmap();

			!Bitmap();

			void LockBits();

			void UnlockBits();

		private:
			const HBITMAP m_hBitmap;
			
			unsigned long m_width;
			unsigned long m_height;

			unsigned long m_scanWidth;
			unsigned short m_planes;
			unsigned short m_bitsPerPixel;

			void* m_scan0;
		};
	}
}

#include "Bitmap.inl"