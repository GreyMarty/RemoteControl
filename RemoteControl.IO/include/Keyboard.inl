namespace RemoteControl 
{
	namespace IO 
	{
		template<typename T>
		void Keyboard::InitInput(INPUT& input, T key)
		{
		}

		template<>
		void Keyboard::InitInput<KeyboardKey>(INPUT& input, KeyboardKey key)
		{
			ZeroMemory(&input, sizeof(INPUT));

			input.type = INPUT_KEYBOARD;

			input.ki.wVk = (WORD)key;
		}

		template<>
		void Keyboard::InitInput<unsigned char>(INPUT& input, unsigned char keyChr)
		{
			ZeroMemory(&input, sizeof(INPUT));

			input.type = INPUT_KEYBOARD;

			input.ki.dwFlags = KEYEVENTF_SCANCODE;
			input.ki.wScan = CharToScancode(keyChr);
		}

		template<>
		void Keyboard::InitInput(INPUT& input, unsigned int scancode)
		{
			ZeroMemory(&input, sizeof(INPUT));

			input.type = INPUT_KEYBOARD;

			input.ki.dwFlags = KEYEVENTF_SCANCODE;
			input.ki.wScan = scancode;
		}

		template <typename T>
		void Keyboard::PressKey(T key) 
		{
			INPUT input;
			InitInput(input, key);

			SendInput(1, &input, sizeof(INPUT));
		}

		template <typename T>
		void Keyboard::ReleaseKey(T key) 
		{
			INPUT input;
			InitInput(input, key);

			input.ki.dwFlags = KEYEVENTF_KEYUP;

			SendInput(1, &input, sizeof(INPUT));
		}

		template <typename T>
		void Keyboard::ClickKey(T key) 
		{
			INPUT input;
			InitInput(input, key);

			SendInput(1, &input, sizeof(INPUT));

			input.ki.dwFlags |= KEYEVENTF_KEYUP;

			SendInput(1, &input, sizeof(INPUT));
		}

		template <typename T>
		void Keyboard::TypeText(array<T>^ keys) 
		{
			int n = keys->Length;

			for (int i = 0; i < n; i++)
			{
				ClickKey<T>(keys[i]);
			}
		}
	}
}
