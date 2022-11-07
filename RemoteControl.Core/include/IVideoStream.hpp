#pragma once
#include "IBitmap.hpp"

namespace RemoteControl
{
	namespace Core
	{
		public interface class IVideoStream
		{
		public:
			IBitmap^ NextFrame();
		};
	}
}