#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <ctime>
#include "gol.h"

class Application
{
public:
	Application(void);
	Application(int generations, std::string loadFile, std::string saveFile,
		bool measure, int x, int y, int seed, std::string check,
		std::string compare);
	~Application(void);

	void run();
private:
	void compare();
	void check();
	void load();
	void createRandom();

	void loadFile(std::string file);
	void saveHeader();
	void saveFooter();
	void generateBoard();

	std::string timeToString(clock_t timer);

	GOL* _gol;
	GOL* _golEndState;

	// params
	int _generations;
	std::string _loadFile;
	std::string _saveFile;
	bool _measure;
	int _x, _y;
	int _seed;
	std::string _check;
	std::string _compare;

	// measurement
	clock_t _my_total_time;
	clock_t _my_save_time;
	clock_t _my_load_time;
	clock_t _my_computation_time;

	bool _measureSection;
	clock_t _total_time;
	clock_t _save_time;
	clock_t _load_time;
	clock_t _computation_time;
};