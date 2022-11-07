#include "pch.h"
#include "Screenshot.hpp"
#include "WindowsScreenRecorder.hpp"

using namespace RemoteControl::Core;

namespace RemoteControl 
{
	namespace IO 
	{
		WindowsScreenRecorder::WindowsScreenRecorder() 
		{
			m_hScreenDc = GetDC(nullptr);
			m_hMemoryDc = CreateCompatibleDC(m_hScreenDc);

			m_screenWidth = GetDeviceCaps(m_hScreenDc, HORZRES);
			m_screenHeight = GetDeviceCaps(m_hScreenDc, VERTRES);
		}

		IBitmap^ WindowsScreenRecorder::NextFrame() 
		{			
			HBITMAP hBitmap = CreateCompatibleBitmap(m_hScreenDc, m_screenWidth, m_screenHeight);
			HBITMAP hOldBitmap = static_cast<HBITMAP>(SelectObject(m_hMemoryDc, hBitmap));
			
			BitBlt(m_hMemoryDc, 0, 0, m_screenWidth, m_screenHeight, m_hScreenDc, 0, 0, SRCCOPY);
			hBitmap = static_cast<HBITMAP>(SelectObject(m_hMemoryDc, hOldBitmap));

			return gcnew Screenshot(static_cast<System::IntPtr>(hBitmap), static_cast<System::IntPtr>(m_hScreenDc));
		}
	}
}