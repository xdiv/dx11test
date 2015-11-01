#pragma once

#include <windows.h>
#include <iostream>
#include <sstream>

class Timer
{
private:
	INT64 frequency;
	float ticksPerMs;
	INT64 startTime;
	float frameTime;

	//std::ostringstream os_;

private:
	Timer(Timer&) {};
public:
	Timer();
	~Timer();

	bool Initialize();
	void Frame();

	float GetTime();
};

