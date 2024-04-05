#include "Game.h"

#include <iostream>




int main(int argc, char** argv)
{
	for (int i = 0; i < argc; ++i)
        std::cout << argv[i] << " ";
    std::cout<<std::endl;
	{
		Game gm(1280, 720, "gam");
		gm.Run();
	}
}
