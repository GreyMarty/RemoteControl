#pragma once

#include <inttypes.h>

#include "ICommandParser.hpp"

namespace RemoteControl 
{
	namespace IO 
	{
		namespace Manipulation 
		{
			public ref class CommandStreamParser : public ICommandParser 
			{
			public:
				virtual property bool CanParse 
				{
					bool get();
				}


				CommandStreamParser(System::IO::Stream^ stream);

				virtual ICommand^ ParseNext();

			private:
				System::IO::Stream^ m_stream;
			};
		}
	}
}

#include "CommandStreamParser.inl"