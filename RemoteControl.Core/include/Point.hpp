#pragma once

namespace RemoteControl 
{
	namespace Core
	{
		generic <typename T>
		public interface class IPoint 
		{
		public:
			property T X 
			{
				virtual T get() = 0;
				virtual void set(T val) = 0;
			}

			property T Y 
			{
				virtual T get() = 0;
				virtual void set(T val) = 0;
			}
		};

		public value class PointI : public IPoint<int>
		{
		public:
			property int X 
			{
				int get() override;
				void set(int val) override;
			}

			property int Y 
			{
				int get() override;
				void set(int val) override;
			}

			PointI(int x, int y);

		private:
			int m_x;
			int m_y;
		};

		public value class PointF : public IPoint<float>
		{
		public:
			property float X
			{
				float get() override;
				void set(float val) override;
			}

			property float Y
			{
				float get() override;
				void set(float val) override;
			}

			PointF(float x, float y);

		private:
			float m_x;
			float m_y;
		};

		public value class PointU : public IPoint<unsigned int>
		{
		public:
			property unsigned int X
			{
				unsigned int get() override;
				void set(unsigned int val) override;
			}

			property unsigned int Y
			{
				unsigned int get() override;
				void set(unsigned int val) override;
			}

			PointU(unsigned int x, unsigned int y);

		private:
			int m_x;
			int m_y;
		};
	}
}

#include "Point.inl"