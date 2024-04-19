#include "Timer.h"

#include <chrono>
#include <string>
#include <sstream>

#include "Logs.h"


Timer::Timer()
{}
Timer::Timer(Logger* logger, std::string name)
{
	init(logger, name);
}
Timer::~Timer()
{
	if(!_ended)
		end();
}


void Timer::init(Logger* logger, std::string name)
{
	_name = name;
	_deltaT = 0;
	_ended = false;
	_start = std::chrono::high_resolution_clock::now();
	_log = logger;
}

void Timer::end()
{
#ifdef ENABLE_TIMER
	(*_log)[TIMER] << "[ " << _name << " ] " << GetTimeTaken() << " microseconds\n";
#endif
	_ended = true;
}

void Timer::pause()
{
	auto now = std::chrono::high_resolution_clock::now();
	_deltaT += std::chrono::duration_cast<std::chrono::microseconds>(now - _start).count();
}

void Timer::resume()
{
	_start = std::chrono::high_resolution_clock::now();
}

void Timer::SetLogLevel(unsigned int loglevel)
{}

void Timer::SetTargetTime(unsigned long targetTime)
{}

unsigned long Timer::TimeRemaining()
{
	return 0;
}

unsigned long Timer::GetTimeTaken()
{
	_end = std::chrono::high_resolution_clock::now();
	unsigned long millis = std::chrono::duration_cast<std::chrono::microseconds>(_end - _start).count();
	return millis+_deltaT;
}
