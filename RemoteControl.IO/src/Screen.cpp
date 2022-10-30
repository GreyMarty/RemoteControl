#include "pch.h"
#include "windows.h"
#include "Screen.hpp"

using namespace RemoteControl::Core;

namespace RemoteControl 
{
	namespace IO 
	{
		Screenshot^ Screen::Capture() 
		{
			HDC hScreenDC = GetDC(nullptr);
			HDC hMemoryDC = CreateCompatibleDC(hScreenDC);

			int width = GetDeviceCaps(hScreenDC, HORZRES);
			int height = GetDeviceCaps(hScreenDC, VERTRES);
			
			HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, width, height);
			HBITMAP hOldBitmap = static_cast<HBITMAP>(SelectObject(hMemoryDC, hBitmap));
			
			BitBlt(hMemoryDC, 0, 0, width, height, hScreenDC, 0, 0, SRCCOPY);
			
			hBitmap = static_cast<HBITMAP>(SelectObject(hMemoryDC, hOldBitmap));
			
			DeleteDC(hMemoryDC);
			DeleteDC(hScreenDC);

			return gcnew Screenshot(static_cast<System::IntPtr>(hBitmap), static_cast<System::IntPtr>(nullptr));
		}
	}
}