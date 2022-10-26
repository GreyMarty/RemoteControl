#pragma once

namespace RemoteControl 
{
	namespace Core 
	{
		inline int PointI::X::get()
		{
			return m_x;
		}

		inline void PointI::X::set(int val) 
		{
			m_x = val;
		}

		inline int PointI::Y::get()
		{
			return m_y;
		}

		inline void PointI::Y::set(int val)
		{
			m_y = val;
		}


		inline float PointF::X::get()
		{
			return m_x;
		}

		inline void PointF::X::set(float val)
		{
			m_x = val;
		}

		inline float PointF::Y::get()
		{
			return m_y;
		}

		inline void PointF::Y::set(float val)
		{
			m_y = val;
		}


		inline unsigned int PointU::X::get()
		{
			return m_x;
		}

		inline void PointU::X::set(unsigned int val)
		{
			m_x = val;
		}

		inline unsigned int PointU::Y::get()
		{
			return m_y;
		}

		inline void PointU::Y::set(unsigned int val)
		{
			m_y = val;
		}
	}
}