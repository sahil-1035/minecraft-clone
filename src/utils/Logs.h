#pragma once
#include <iostream>
#include <string>
#include <unordered_map>

enum LogType { INFO, WARN, ERROR };

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
		std::cout << "[" << logString.at(logtype) << "] "
			<< "[" << _catagory << "] ";
		return *this;
	}
	template<typename T>
	Logger& operator<<(T message)
	{
		std::cout << message;
		return *this;
	}
private:
	std::string _catagory;
	const std::unordered_map<LogType, std::string> logString = {
		{ INFO, "INFO" },
		{ WARN, "WARN" },
		{ ERROR, "ERROR" }
	};
};
