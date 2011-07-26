#include "UTimer.h"

UTimer::UTimer(void)
{
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_llFrequency);
	QueryPerformanceCounter((LARGE_INTEGER*)&m_llStartTick);

	m_bRunning = false;
}

void UTimer::Start( void )
{
	if( !m_bRunning )
	{
		m_bRunning = true;

		QueryPerformanceCounter((LARGE_INTEGER*)&m_llStartTick);
	}
}

void UTimer::Reset(void)
{
	m_dElapsedTime = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&m_llStartTick);
}

double UTimer::GetElapsedTime(void)
{
	double dElapsed = 0;

	if (m_bRunning)
	{
		LONGLONG llCurTick;
		QueryPerformanceCounter((LARGE_INTEGER*)&llCurTick);

		dElapsed = (double)((llCurTick - m_llStartTick)) / (double)m_llFrequency;
	}

	return dElapsed;
}