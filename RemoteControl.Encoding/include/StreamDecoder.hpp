#pragma once

#include <inttypes.h>

namespace RemoteControl 
{
	namespace Encoding 
	{
		public ref class StreamDecoder : public Core::IVideoStream 
		{
		public:
			virtual property uint32_t Width
			{
				uint32_t get();
			}

			virtual property uint32_t Height 
			{
				uint32_t get();
			}

			StreamDecoder(System::IO::Stream^ stream);

			~StreamDecoder();

			!StreamDecoder();

			virtual Core::IBitmap^ NextFrame();

			void Flush();

		private:
			uint32_t m_width;
			uint32_t m_height;

			const AVCodec* m_codec;
			AVCodecContext* m_context;
			AVCodecParserContext* m_parser;

			AVPacket* m_packet;
			AVFrame* m_frame;
			AVFrame* m_bitmapFrame;

			System::IO::Stream^ m_stream;

			array<uint8_t>^ m_inbuff;

			uint8_t* m_data0;
			uint8_t* m_data;
			size_t m_dataSize;

			uint64_t m_latencyMilliseconds;

			bool m_parsePacket = false;

			static int DecodeInit(AVCodecContext* context, AVPacket* packet);

			static bool DecodeNext(AVCodecContext* context, AVFrame* frame, AVFrame* bitmapFrame, AVPacket* packet, Core::IBitmap^% outBitmap, bool sync);
		};
	}
}

#include "StreamDecoder.inl"