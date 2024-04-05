#include "File.h"

#include <iostream>
#include <sstream>
#include <fstream>


File::File()
{}

File::File(const char* filePath)
{
	Init(filePath);
}

File::~File()
{}

void File::Init(const char* filePath)
{
	_filePath = filePath;
}

std::string File::ReadFile()
{
	std::string fileContents;
	std::ifstream file(_filePath, std::ios::in);
	if(file.is_open()){
		std::stringstream sstr;
		sstr << file.rdbuf();
		fileContents = sstr.str();
		file.close();
	}else{
		std::cout<<"[ERROR]  Could not open "<<_filePath<<"."<<std::endl;
		getchar();
	}
	return fileContents;
}