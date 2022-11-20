#include "pch.h"
#include "windows.h"
#include "Screen/Screenshot.hpp"
#include "Screen/WindowsScreenRecorder.hpp"

using namespace RemoteControl::Core;

namespace RemoteControl
{
	namespace IO
	{
		namespace Screen
		{
			uint32_t WindowsScreenRecorder::Width::get()
			{
				return m_screenWidth;
			}

			uint32_t WindowsScreenRecorder::Height::get()
			{
				return m_screenHeight;
			}

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
}