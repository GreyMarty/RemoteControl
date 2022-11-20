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
				property int DataSize 
				{
					int get();
				}

				void Execute();

				int WriteTo(System::IO::Stream^ stream);
			};
		}
	}
}