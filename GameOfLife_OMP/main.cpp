#include <iostream>
#include <string>
// for memory leaks
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
//
#include "time.h"
#include "Application.h"

//--generations NUM
//--load NAME (*.gol)
//--save NAME
//--measure
//--dim NUM NUM
//--seed NUM
//--check NAME
//--compare NAME
int main(int argc, char *argv[])
{
	int generations = 0;
	std::string loadFile = "";
	std::string saveFile = "";
	bool measure = false;
	int x = 0;
	int y = 0;
	srand((int) time(0));
	int seed = rand();
	std::string check = "";
	std::string compare = "";

	// check params
	if (argc > 1)
	{
		for (int i = 1; i < argc; ++i)
		{
			if (strncmp(argv[i], "--generations", 13) == 0) generations = atoi(argv[i + 1]);
			if (strncmp(argv[i], "--load", 6) == 0) loadFile = argv[i + 1];
			if (strncmp(argv[i], "--save", 6) == 0) saveFile = argv[i + 1];
			if (strncmp(argv[i], "--measure", 9) == 0) measure = true;
			if (strncmp(argv[i], "--dim", 5) == 0) { x = atoi(argv[i + 1]); y = atoi(argv[i + 2]); }
			if (strncmp(argv[i], "--seed", 6) == 0) seed = atoi(argv[i + 1]);
			if (strncmp(argv[i], "--check", 7) == 0) check = argv[i + 1];
			if (strncmp(argv[i], "--compare", 9) == 0) compare = argv[i + 1];
		}
	}
	else
	{
		std::cout << "Usage: " << argv[0] << std::endl;
		std::cout << " Load a board and simulate" << std::endl;
		std::cout << "   --load NAME" << std::endl;
		std::cout << "   --generations NUM" << std::endl;
		std::cout << "   --save NAME" << std::endl;
		std::cout << "   --measure" << std::endl;
		std::cout << " Create a new random board and simulte" << std::endl;
		std::cout << "   --dim NUM NUM" << std::endl;
		std::cout << "   --seed NUM" << std::endl;
		std::cout << "   --generations NUM" << std::endl;
		std::cout << "   --save NAME" << std::endl;
		std::cout << "   --measure" << std::endl;
		std::cout << " Check file" << std::endl;
		std::cout << "   --check NAME" << std::endl;
		std::cout << " Compare performance" << std::endl;
		std::cout << "   --compare NAME" << std::endl;
	}

	//std::cout<<"***Start***\n";

	Application a(generations, loadFile, saveFile, measure, x, y, seed, check, compare);
	a.run();

	//std::cout<<"***Ende***\n";

	// show memory leaks
	_CrtDumpMemoryLeaks();

	return 0;
}