#include "pch.h"

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavutil/opt.h"
#include "libavutil/avutil.h"
#include "libavutil/imgutils.h"
#include "libswscale/swscale.h"
}

#include "StreamEncoder.hpp"

using namespace System;
using namespace System::IO;
using namespace System::Runtime::InteropServices;
using namespace RemoteControl::Core;

namespace RemoteControl
{
	namespace Encoding
	{
		StreamEncoder::StreamEncoder(IVideoStream^ videoStream) :
			m_videoStream(videoStream)
		{
			m_codec = avcodec_find_encoder(AV_CODEC_ID_H264);

			m_codecContext = avcodec_alloc_context3(m_codec);
			m_codecContext->bit_rate = 400000;
			m_codecContext->width = videoStream->Width;
			m_codecContext->height = videoStream->Height;
			m_codecContext->time_base = { 1, 25 };
			m_codecContext->framerate = { 25, 1 };
			m_codecContext->gop_size = 10;
			m_codecContext->max_b_frames = 1;
			m_codecContext->pix_fmt = AV_PIX_FMT_YUV420P;

			av_opt_set(m_codecContext->priv_data, "preset", "ultrafast", 0);

			avcodec_open2(m_codecContext, m_codec, NULL);

			m_packet = av_packet_alloc();

			m_bitmapFrame = av_frame_alloc();
			m_bitmapFrame->format = AV_PIX_FMT_RGB32;

			m_videoFrame = av_frame_alloc();
			m_videoFrame->format = m_codecContext->pix_fmt;
			m_videoFrame->width = m_codecContext->width;
			m_videoFrame->height = m_codecContext->height;

			av_frame_get_buffer(m_videoFrame, 32);
		}

		StreamEncoder::~StreamEncoder() 
		{
			pin_ptr<AVCodecContext*> p_codecContext = &m_codecContext;
			pin_ptr<AVFrame*> p_bitmapFrame = &m_bitmapFrame;
			pin_ptr<AVFrame*> p_videoFrame = &m_videoFrame;
			pin_ptr<AVPacket*> p_packet = &m_packet;

			avcodec_free_context(p_codecContext);
			av_frame_free(p_bitmapFrame);
			av_frame_free(p_videoFrame);
			av_packet_free(p_packet);
		}

		StreamEncoder::!StreamEncoder() 
		{
			this->~StreamEncoder();
		}

		void StreamEncoder::EncodeFrame(Stream^ outStream)
		{
			IBitmap^ bitmap = m_videoStream->NextFrame();

			bitmap->LockBits();
			
			m_bitmapFrame->width = bitmap->Width;
			m_bitmapFrame->height = bitmap->Height;
			m_bitmapFrame->data[0] = static_cast<uint8_t*>(bitmap->Scan0.ToPointer());
			m_bitmapFrame->linesize[0] = bitmap->Stride;

			SwsContext* swsContext = sws_getContext(
				m_bitmapFrame->width,
				m_bitmapFrame->height,
				(AVPixelFormat)m_bitmapFrame->format,
				m_videoFrame->width,
				m_videoFrame->height,
				(AVPixelFormat)m_videoFrame->format,
				SWS_FAST_BILINEAR,
				NULL,
				NULL,
				NULL
			);

			sws_scale(swsContext, m_bitmapFrame->data, m_bitmapFrame->linesize, 0, m_bitmapFrame->height, m_videoFrame->data, m_videoFrame->linesize);

			bitmap->UnlockBits();

			m_videoFrame->pts = m_iframe++;

			Encode(m_codecContext, m_videoFrame, m_packet, outStream);
		}

		void StreamEncoder::Flush(Stream^ outStream)
		{
			Encode(m_codecContext, NULL, m_packet, outStream);
		}

		void StreamEncoder::Encode(AVCodecContext* encodeContext, AVFrame* frame, AVPacket* packet, Stream^ outStream) 
		{
			int ret;

			ret = avcodec_send_frame(encodeContext, frame);

			if (ret < 0)
			{
				throw gcnew System::Exception("Could not send frame to context!");
			}

			while (ret >= 0)
			{
				ret = avcodec_receive_packet(encodeContext, packet);

				if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
				{
					return;
				}
				else if (ret < 0)
				{
					throw gcnew System::Exception("Error during encoding!");
				}

				array<uint8_t>^ buffer = gcnew array<uint8_t>(packet->size);
				Marshal::Copy(static_cast<IntPtr>(packet->data), buffer, 0, packet->size);

				outStream->Write(buffer, 0, buffer->Length);
				
				av_packet_unref(packet);
			}
		}
	}
}