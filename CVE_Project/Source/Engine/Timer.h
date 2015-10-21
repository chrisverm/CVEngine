#ifndef TIMER_H
#define TIMER_H

#include "CVEDataTypes.h"

namespace CVE
{
	namespace System
	{
		class Timer
		{
		public:
			Timer( void );

			f64 totalTime( void ) const;
			f64 deltaTime( void ) const;

			void start( void );
			void stop( void );
			void tick( void );
			void reset( void );

		private:
			f64 m_secondsPerCount;
			f64 m_deltaTime;

			i64 m_baseTime;
			i64 m_pausedTime;
			i64 m_stopTime;
			i64 m_prevTime;
			i64 m_currTime;

			bool m_stopped;
		};
	}
}

#endif