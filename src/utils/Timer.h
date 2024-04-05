#pragma once

#include <chrono>
#include <string>


class Timer
{
public:
	Timer();
	Timer(std::string name);
	~Timer();

	void init(std::string name);
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

	bool _ended;
};
