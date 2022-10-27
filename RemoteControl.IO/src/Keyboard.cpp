#include "pch.h"
#include "Keyboard.hpp"

namespace RemoteControl 
{
	namespace IO 
	{
		void Keyboard::PressKey(KeyboardKey key) 
		{
			PressKey<KeyboardKey>(key);
		}

		void Keyboard::PressKey(unsigned char keyChr)
		{
			PressKey<unsigned char>(keyChr);
		}

		void Keyboard::PressKey(unsigned int scancode) 
		{
			PressKey<unsigned int>(scancode);
		}

		void Keyboard::ReleaseKey(KeyboardKey key) 
		{
			ReleaseKey<KeyboardKey>(key);
		}

		void Keyboard::ReleaseKey(unsigned char keyChr)
		{
			ReleaseKey<unsigned char>(keyChr);
		}

		void Keyboard::ReleaseKey(unsigned int scancode) 
		{
			ReleaseKey<unsigned int>(scancode);
		}

		void Keyboard::ClickKey(KeyboardKey key) 
		{
			ClickKey<KeyboardKey>(key);
		}

		void Keyboard::ClickKey(unsigned char keyChr)
		{
			ClickKey<unsigned char>(keyChr);
		}

		void Keyboard::ClickKey(unsigned int scancode) 
		{
			ClickKey<unsigned int>(scancode);
		}

		void Keyboard::TypeText(array<KeyboardKey>^ keys) 
		{
			TypeText<KeyboardKey>(keys);
		}

		void Keyboard::TypeText(array<unsigned char>^ text) 
		{
			TypeText<unsigned char>(text);
		}

		void Keyboard::TypeText(array<unsigned int>^ scancodes) 
		{
			TypeText<unsigned int>(scancodes);
		}

		unsigned int Keyboard::CharToScancode(unsigned char keyChr) 
		{
			return 0;
		}

		unsigned int Keyboard::KeyToScancode(KeyboardKey key) 
		{
			return MapVirtualKeyW((UINT)key, MAPVK_VK_TO_VSC);
		}
	}
}