#pragma once

#include "ICommand.hpp"

namespace RemoteControl 
{
	namespace IO 
	{
		namespace Manipulation 
		{
			public interface class ICommandParser 
			{
			public:
				ICommand^ ParseNext();
			};
		}
	}
}