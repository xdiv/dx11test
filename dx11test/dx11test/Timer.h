#pragma once

#include <windows.h>

class Timer
{
private:
	INT64 frequency;
	float ticksPerMs;
	INT64 startTime;
	float frameTime;

private:
	Timer(Timer&) {};
public:
	Timer();
	~Timer();

	bool Initialize();
	void Frame();

	float GetTime();
};

