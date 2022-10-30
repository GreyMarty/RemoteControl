#include "pch.h"
#include "Screenshot.hpp"

namespace RemoteControl
{
	namespace Core
	{
		Screenshot::Screenshot(const HBITMAP hBitmap, const HWND hWnd) :
			m_hBitmap(hBitmap),
			m_hWnd(hWnd)
		{
			HDC hDc = GetDC(hWnd);

			BITMAPINFO bitmapInfo = { 0 };
			bitmapInfo.bmiHeader.biSize = sizeof(bitmapInfo.bmiHeader);

			GetDIBits(hDc, m_hBitmap, 0, 0, NULL, &bitmapInfo, DIB_RGB_COLORS);

			m_width = bitmapInfo.bmiHeader.biWidth;
			m_height = bitmapInfo.bmiHeader.biHeight;

			m_bitsPerPixel = bitmapInfo.bmiHeader.biBitCount;

			m_sizeBytes = bitmapInfo.bmiHeader.biSizeImage;

			m_stride = ((m_width * m_bitsPerPixel + 31) / 32) * 4;

			DeleteDC(hDc);
		}

		Screenshot::Screenshot(System::IntPtr hBitmap, System::IntPtr hWnd) :
			Screenshot(static_cast<HBITMAP>(hBitmap.ToPointer()), static_cast<HWND>(hWnd.ToPointer()))
		{
		}

		Screenshot::~Screenshot()
		{
			DeleteObject(m_hBitmap);
			UnlockBits();
		}

		Screenshot::!Screenshot()
		{
			this->~Screenshot();
		}

		void Screenshot::LockBits()
		{
			if (m_scan0)
			{
				return;
			}

			m_scan0 = new char[SizeBytes];

			HDC hDc = GetDC(m_hWnd);

			BITMAPINFO bitmapInfo = { 0 };
			bitmapInfo.bmiHeader.biSize = sizeof(bitmapInfo.bmiHeader);

			GetDIBits(hDc, m_hBitmap, 0, 0, NULL, &bitmapInfo, DIB_RGB_COLORS);

			bitmapInfo.bmiHeader.biCompression = BI_RGB;
			bitmapInfo.bmiHeader.biHeight *= -1;

			GetDIBits(hDc, m_hBitmap, 0, bitmapInfo.bmiHeader.biHeight, m_scan0, &bitmapInfo, DIB_RGB_COLORS);

			DeleteDC(hDc);
		}

		void Screenshot::UnlockBits()
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