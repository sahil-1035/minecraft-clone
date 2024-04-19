#pragma once

#include <chrono>
#include <string>

#include "Logs.h"


class Timer
{
public:
	Timer();
	Timer(Logger* logger, std::string name);
	~Timer();

	void init(Logger* logger, std::string name);
	void end();

	void pause();
	void resume();

	void SetLogLevel(unsigned int loglevel);
	void SetTargetTime(unsigned long targetime);
	unsigned long TimeRemaining();
	unsigned long GetTimeTaken();
private:
	std::string _name;
	std::chrono::time_point<std::chrono::high_resolution_clock> _start;
	std::chrono::time_point<std::chrono::high_resolution_clock> _end;

	unsigned long _deltaT;
	Logger* _log;

	bool _ended;
};
