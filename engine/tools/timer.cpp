#include "timer.hpp"

#include <Windows.h>

#pragma comment( lib, "Winmm.lib")



using namespace heimdall::engine;



tools::time_period::time_period( unsigned int interval )
	: interval( interval )
{
	if (interval != 0)
	{
		tools::timecaps tc = get_timecaps();
		interval = min( max( tc.min_period, interval ), tc.max_period );
		timeBeginPeriod( interval );
	}
}

tools::time_period::time_period( const time_period& other ) noexcept
	: interval( other.interval )
{
	if (interval != 0)
	{
		timeBeginPeriod( interval );
	}
}

tools::timecaps tools::time_period::get_timecaps()
{
	TIMECAPS tc;
	UINT     wTimerRes;

	if (timeGetDevCaps( &tc, sizeof( TIMECAPS ) ) != TIMERR_NOERROR)
	{
		// TODO
		// Error; application can't continue.
		throw "TODO. Timecaps error";
	}

	return tools::timecaps { tc.wPeriodMin, tc.wPeriodMax };
}

tools::time_period::~time_period()
{
	if (interval != 0)
	{
		timeEndPeriod( interval );
	}
}
