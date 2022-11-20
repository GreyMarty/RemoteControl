#pragma once
#include "KeyboardKey.hpp"


namespace RemoteControl
{
	namespace IO
	{
		namespace Keyboard
		{
			public ref class Keyboard abstract sealed
			{
			public:
				static void PressKey(KeyboardKey key);

				static void PressKey(unsigned char keyChr);

				static void PressKey(unsigned int scancode);

				static void ReleaseKey(KeyboardKey key);

				static void ReleaseKey(unsigned char keyChr);

				static void ReleaseKey(unsigned int scancode);

				static void ClickKey(KeyboardKey key);

				static void ClickKey(unsigned char keyChr);

				static void ClickKey(unsigned int scancode);

				static void TypeText(array<KeyboardKey>^ keys);

				static void TypeText(array<unsigned char>^ keyChars);

				static void TypeText(array<unsigned int>^ scancodes);

			private:
				static unsigned int KeyToScancode(KeyboardKey key);

				static unsigned int CharToScancode(unsigned char keyChr);

				template <typename T>
				static void InitInput(INPUT& input, T key);

				template <>
				static void InitInput<KeyboardKey>(INPUT& input, KeyboardKey key);

				template <>
				static void InitInput<unsigned char>(INPUT& input, unsigned char chrKey);

				template <>
				static void InitInput<unsigned int>(INPUT& input, unsigned int chrKey);

				template <typename T>
				static void PressKey(T key);

				template <typename T>
				static void ReleaseKey(T key);

				template <typename T>
				static void ClickKey(T key);

				template <typename T>
				static void TypeText(array<T>^ keys);
			};
		}
	}
}

#include "Keyboard.inl"