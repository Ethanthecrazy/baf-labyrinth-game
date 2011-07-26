#ifndef UTIMER_H_
#define UTIMER_H_

#include <Windows.h>

class UTimer
{
private:

	bool m_bRunning;

	LONGLONG m_llStartTick;
	LONGLONG m_llFrequency;

	double m_dElapsedTime;


public:

	UTimer( void );

	void Start( void );

	void Reset( void );

	double GetElapsedTime( void );

};

#endif