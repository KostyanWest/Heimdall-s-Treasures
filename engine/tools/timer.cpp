#include <Windows.h>
#include "timer.hpp"

#pragma comment( lib, "Winmm.lib")



using namespace heimdall::engine;



tools::timecaps tools::timer::get_timecaps()
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

void tools::timer::set_tick_interval( unsigned int tick_interval )
{
	reset_tick_interval();
	if (tick_interval != 0)
	{
		tools::timecaps tc = get_timecaps();
		tick_interval = min( max( tc.min_period, tick_interval ), tc.max_period );
		timeBeginPeriod( tick_interval );
		this->tick_interval = tick_interval;
	}
}

void tools::timer::reset_tick_interval() noexcept
{
	if (tick_interval != 0)
	{
		timeEndPeriod( tick_interval );
		tick_interval = 0;
	}
}