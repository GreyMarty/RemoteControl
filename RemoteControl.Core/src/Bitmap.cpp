#include "pch.h"
#include "windows.h"
#include "Bitmap.hpp"

namespace RemoteControl 
{
	namespace Core 
	{
		Bitmap::Bitmap(const HBITMAP hBitmap) : 
			m_hBitmap(hBitmap)
		{
			BITMAP bitmap;
			GetObject(hBitmap, sizeof(BITMAP), &bitmap);

			m_width = bitmap.bmWidth;
			m_height = bitmap.bmHeight;

			m_scanWidth = bitmap.bmWidthBytes;
			m_planes = bitmap.bmPlanes;
			m_bitsPerPixel = bitmap.bmBitsPixel;
		}

		Bitmap::Bitmap(System::IntPtr hBitmap) :
			Bitmap(static_cast<HBITMAP>(hBitmap.ToPointer()))
		{
		}

		Bitmap::~Bitmap() 
		{
			CloseHandle(m_hBitmap);
			UnlockBits();
		}

		Bitmap::!Bitmap()
		{
			this->~Bitmap();
		}

		void Bitmap::LockBits() 
		{
			if (m_scan0) 
			{
				return;
			}

			m_scan0 = new char[SizeBytes];
			GetBitmapBits(m_hBitmap, SizeBytes, m_scan0);
		}

		void Bitmap::UnlockBits() 
		{
			if (!m_scan0) 
			{
				return;
			}

			delete[] m_scan0;
			m_scan0 = nullptr;
		}
	}
}