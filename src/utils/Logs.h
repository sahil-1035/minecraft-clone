#pragma once
#include <iostream>
#include <string>
#include <unordered_map>

enum LogType { TIMER, INFO, WARN, ERROR };

#ifdef ENABLE_LOG
	#define COUT std::cout
#else
	#define COUT if( false ) std::cout
#endif

class Logger
{
public:
	Logger()
	{}
	Logger(std::string catagory)
	{
		Init(catagory);
	}
	void Init(std::string catagory)
	{
		_catagory = catagory;
	}
	Logger& operator[](LogType logtype)
	{
		COUT << "[ " << logString.at(logtype) << " ] "
			<< "[ " << _catagory << " ] ";
		return *this;
	}
	template<typename T>
	Logger& operator<<(T message)
	{
		COUT << message;
		return *this;
	}
private:
	std::string _catagory;
	const std::unordered_map<LogType, std::string> logString = {
		{ INFO, " INFO" },
		{ WARN, " WARN" },
		{ TIMER, "TIMER"},
		{ ERROR, "ERROR" }
	};
};
