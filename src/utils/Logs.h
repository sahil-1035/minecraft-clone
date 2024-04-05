#pragma once
#include <iostream>
#include <string>

#define DEBUG
#define LOG_MAX 0

static void logFunc(std::string message, int loglevel)
{
	if(loglevel<LOG_MAX)
		std::cout<<message<<std::endl;
}

#ifdef DEBUG
	#define Log(x,y) logFunc(x,y)
#else
	#define Log(x,y)
#endif // DEBUG
