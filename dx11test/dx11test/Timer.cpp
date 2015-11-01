#include "Timer.h"


Timer::Timer()
{
}


Timer::~Timer()
{
}

bool Timer::Initialize()
{
	// Check to see if this system supports high performance timers.
	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
	if (frequency == 0)
	{
		return false;
	}

	// Find out how many times the frequency counter ticks every millisecond.
	//ticksPerMs = (float)(frequency / 1000);

	ticksPerMs = frequency;

	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

	return true;
}

void Timer::Frame()
{
	INT64 currentTime;
	float timeDifference;


	QueryPerformanceCounter((LARGE_INTEGER*)& currentTime);

	timeDifference = (float)(currentTime - startTime);

	frameTime = timeDifference / ticksPerMs;

	startTime = currentTime;

	//os_ << frameTime << "\n";
	//OutputDebugStringA(os_.str().c_str());
	return;
}

float Timer::GetTime()
{
	return frameTime;
}
