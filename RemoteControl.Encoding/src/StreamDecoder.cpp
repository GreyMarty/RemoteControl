#include "pch.h"

#include <iostream>
#include <chrono>

extern "C" 
{
#include "libavcodec/avcodec.h"
#include "libswscale/swscale.h"
#include "libavutil/opt.h"
}

#include "StreamDecoder.hpp"
#include "MemoryBitmap.hpp"

using namespace System;
using namespace System::IO;
using namespace System::Runtime::InteropServices;
using namespace RemoteControl::Core;
using namespace std::chrono;

namespace RemoteControl 
{
	namespace Encoding 
	{
		const uint32_t INBUF_SIZE = 4096;

		StreamDecoder::StreamDecoder(Stream^ stream) :
			m_stream(stream)
		{
			m_codec = avcodec_find_decoder(AV_CODEC_ID_H264);
			//m_codec = avcodec_find_decoder_by_name("h264_nvenc");

			m_parser = av_parser_init(m_codec->id);

			m_context = avcodec_alloc_context3(m_codec);

			avcodec_open2(m_context, m_codec, NULL);

			m_packet = av_packet_alloc();
			m_frame = av_frame_alloc();
			m_bitmapFrame = av_frame_alloc();

			m_inbuff = gcnew array<uint8_t>(INBUF_SIZE + AV_INPUT_BUFFER_PADDING_SIZE);
		}

		StreamDecoder::~StreamDecoder() 
		{
			pin_ptr<AVCodecContext*> p_codecContext = &m_context;
			pin_ptr<AVFrame*> p_frame = &m_frame;
			pin_ptr<AVFrame*> p_bitmapFrame = &m_bitmapFrame;
			pin_ptr<AVPacket*> p_packet = &m_packet;

			av_parser_close(m_parser);
			avcodec_free_context(p_codecContext);
			av_frame_free(p_frame);
			av_frame_free(p_bitmapFrame);
			av_packet_free(p_packet);
		}

		StreamDecoder::!StreamDecoder() 
		{
			this->~StreamDecoder();
		}

		IBitmap^ StreamDecoder::NextFrame() 
		{
			if (m_dataSize <= 0) 
			{
				Array::Clear(m_inbuff, 0, m_inbuff->Length);
				m_dataSize = m_stream->Read(m_inbuff);

				if (m_data0) 
				{
					delete[] m_data0;
					m_data0 = nullptr;
				}

				m_data0 = new uint8_t[m_dataSize];
				m_data = m_data0;
				Marshal::Copy(m_inbuff, 0, static_cast<IntPtr>(m_data), m_dataSize);
			}

			if (!m_parsePacket)
			{
				int ret = av_parser_parse2(
					m_parser,
					m_context,
					&m_packet->data,
					&m_packet->size,
					m_data,
					m_dataSize,
					AV_NOPTS_VALUE,
					AV_NOPTS_VALUE,
					0
				);

				if (ret < 0)
				{
					throw gcnew Exception("Error while parsing!");
				}

				m_data += ret;
				m_dataSize -= ret;

				if (m_packet->size)
				{
					milliseconds timestamp;
					memcpy(&timestamp, m_packet->data + m_packet->size - sizeof(milliseconds), sizeof(milliseconds));

					m_packet->size -= sizeof(milliseconds);

					m_latencyMilliseconds = (duration_cast<milliseconds>(system_clock::now().time_since_epoch()) - timestamp).count();
					std::cout << "Latency: " << m_latencyMilliseconds << "ms" << std::endl;

					DecodeInit(m_context, m_packet);
					m_parsePacket = true;
				}
			}

			IBitmap^ bitmap;

			if (m_parsePacket)
			{
				bool sync = m_latencyMilliseconds > 75;
				m_parsePacket = DecodeNext(m_context, m_frame, m_bitmapFrame, m_packet, bitmap, sync);

				if (bitmap)
				{
					m_width = bitmap->Width;
					m_height = bitmap->Height;
				}
			}

			return bitmap;
		}

		void StreamDecoder::Flush() 
		{

		}

		int StreamDecoder::DecodeInit(AVCodecContext* context, AVPacket* packet) 
		{
			int ret = avcodec_send_packet(context, packet);

			if (ret < 0) 
			{
				throw gcnew Exception("Error sending packet for decoding!");
			}

			return ret;
		}

		bool StreamDecoder::DecodeNext(AVCodecContext* context, AVFrame* frame, AVFrame* bitmapFrame, AVPacket* packet, IBitmap^% outBitmap, bool sync)
		{
			int ret = avcodec_receive_frame(context, frame);

			if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) 
			{
				return false;
			}

			if (ret < 0)
			{
				throw gcnew Exception("Error during decoding!");
			}

			if (sync) 
			{
				av_frame_unref(bitmapFrame);
				return true;
			}

			bitmapFrame->format = AV_PIX_FMT_RGB32;
			bitmapFrame->width = frame->width;
			bitmapFrame->height = frame->height;
			
			av_frame_get_buffer(bitmapFrame, 32);
			

			SwsContext* swsContext = sws_getContext(
				frame->width,
				frame->height,
				(AVPixelFormat)frame->format,
				bitmapFrame->width,
				bitmapFrame->height,
				(AVPixelFormat)bitmapFrame->format,
				SWS_FAST_BILINEAR,
				NULL,
				NULL,
				NULL
			);

			sws_scale(
				swsContext,
				frame->data,
				frame->linesize,
				0,
				frame->height,
				bitmapFrame->data,
				bitmapFrame->linesize
			);

			outBitmap = gcnew MemoryBitmap(bitmapFrame->width, bitmapFrame->height, bitmapFrame->linesize[0], 32, static_cast<IntPtr>(bitmapFrame->data[0]));

			av_frame_unref(bitmapFrame);

			return true;
		}
	}
}