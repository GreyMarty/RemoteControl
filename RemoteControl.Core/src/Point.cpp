#include "pch.h"
#include "Point.hpp"

namespace RemoteControl 
{
	namespace Core 
	{
		PointI::PointI(int x, int y) :
			m_x(x),
			m_y(y) 
		{
		}

		PointF::PointF(float x, float y) :
			m_x(x),
			m_y(y)
		{
		}

		PointU::PointU(unsigned int x, unsigned int y) :
			m_x(x),
			m_y(y)
		{
		}
	}
}