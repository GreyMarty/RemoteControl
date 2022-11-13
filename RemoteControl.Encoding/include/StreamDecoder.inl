#pragma once

namespace RemoteControl 
{
	namespace Encoding 
	{
		inline uint32_t StreamDecoder::Width::get()
		{
			return m_width;
		}

		inline uint32_t StreamDecoder::Height::get()
		{
			return m_height;
		}
	}
}