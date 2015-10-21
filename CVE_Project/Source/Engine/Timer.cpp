#include "Timer.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace CVE
{
	namespace System
	{
		Timer::Timer( void )
		{
			m_secondsPerCount = 0.0;
			m_deltaTime = -1.0;
			m_baseTime = 0;
			m_pausedTime = 0;
			m_prevTime = 0;
			m_currTime = 0;
			m_stopped = false;

			i64 countsPerSec;
			QueryPerformanceFrequency( (LARGE_INTEGER*)&countsPerSec );
			m_secondsPerCount = 1.0 / (f64)countsPerSec;
		}

		f64 Timer::totalTime( void ) const
		{
			//                     |<--paused time-->|
			// ----*---------------*-----------------*------------*------------*------> time
			//  mBaseTime       mStopTime        startTime     mStopTime    mCurrTime
			if ( m_stopped )
			{
				return ( ( m_stopTime - m_pausedTime ) - m_baseTime ) * m_secondsPerCount;
			}
			//                     |<--paused time-->|
			// ----*---------------*-----------------*------------*------> time
			//  mBaseTime       mStopTime        startTime     mCurrTime
			else
			{
				return ( ( m_currTime - m_pausedTime ) - m_baseTime ) * m_secondsPerCount;
			}

			return 0.0f;
		}

		f64 Timer::deltaTime( void ) const
		{
			return m_deltaTime;
		}

		void Timer::start( void )
		{
			if ( m_stopped )
			{
				i64 startTime;
				QueryPerformanceCounter( (LARGE_INTEGER*)&startTime );

				m_pausedTime += ( startTime - m_stopTime );

				m_prevTime = startTime;
				m_stopTime = 0;
				m_stopped = false;
			}
		}

		void Timer::stop( void )
		{
			if ( !m_stopped )
			{
				i64 currTime;
				QueryPerformanceCounter( (LARGE_INTEGER*)&currTime );

				m_stopTime = currTime;
				m_stopped = true;
			}
		}

		void Timer::tick( void )
		{
			if ( m_stopped )
			{
				m_deltaTime = 0.0;
				return;
			}

			i64 currTime;
			QueryPerformanceCounter( (LARGE_INTEGER*)&currTime );
			m_currTime = currTime;

			m_deltaTime = ( m_currTime * m_prevTime ) * m_secondsPerCount;
			if ( m_deltaTime < 0.0 ) m_deltaTime = 0.0;

			m_prevTime = m_currTime;
		}

		void Timer::reset( void )
		{
			i64 currTime;
			QueryPerformanceCounter( (LARGE_INTEGER*)&currTime );

			m_baseTime = currTime;
			m_prevTime = currTime;
			m_stopTime = 0;
			m_stopped = false;
		}
	}
}