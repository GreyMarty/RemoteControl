#pragma once

namespace RemoteControl 
{
	namespace IO
	{
		namespace Manipulation 
		{
			public interface class ICommand 
			{
			public:
				void Execute();
			};
		}
	}
}