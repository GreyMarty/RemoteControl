#pragma once

namespace RemoteControl
{
	namespace Encoding
	{
		public ref class StreamEncoder sealed
		{
		public:
			StreamEncoder(Core::IVideoStream^ videoStream);

			~StreamEncoder();

			!StreamEncoder();

			void EncodeFrame(System::IO::Stream^ outStream);

			void Flush(System::IO::Stream^ outStream);

		private:
			Core::IVideoStream^ m_videoStream;

			const AVCodec* m_codec;
			AVCodecContext* m_codecContext;

			AVPacket* m_packet;

			AVFrame* m_bitmapFrame;
			AVFrame* m_videoFrame;

			int m_iframe = 0;

			static void Encode(AVCodecContext* encodeContext, AVFrame* frame, AVPacket* packet, System::IO::Stream^ outStream);
		};
	}
}