#include "application.h"

Application::Application(void)
{
	_gol = 0;
	_golEndState = 0;
}

Application::Application(int generations, std::string loadFile, std::string saveFile,
	bool measure, int x, int y, int seed, std::string check,
	std::string compare)
{
	_generations = generations;
	_loadFile = loadFile;
	_saveFile = saveFile;
	_measure = measure;
	_x = x;
	_y = y;
	_seed = seed;
	_check = check;
	_compare = compare;

	_total_time = 0;
	_save_time = 0;
	_load_time = 0;
	_computation_time = 0;
	_measureSection = false;

	_my_total_time = 0;
	_my_save_time = 0;
	_my_load_time = 0;
	_my_computation_time = 0;

	_gol = 0;
	_golEndState = 0;
}

Application::~Application(void)
{
#ifndef RELEASE
	assert(_gol);
#endif
	if (_gol) delete _gol;
#ifndef RELEASE
	// assert(_golEndState);
#endif
	if (_golEndState) delete _golEndState;
}

void Application::run()
{
	// 4 possibilities
	// --> load file and compute generations
	// --> create random board and compute generations
	// --> load checkfile and compare generations
	// --> load comparefile and compare generations
	if (_compare != "")
	{
		// compares given measurement with own one
		compare();
	}
	else {
		if (_check != "") {
			// checks if the given file is correct
			check();
		}
		else {
			_my_load_time /*= _my_total_time*/ = clock();
			if (_loadFile != "") {
				// loads a board from a File
				load();
			}
			else {
				// creates a random board of the given dimension
				createRandom();
			}
			//if(_saveFile!="")
			//{
			_my_load_time = clock() - _my_load_time;
			_my_save_time = clock();

			// save the board to file --> header
			saveHeader();

			_my_save_time = clock() - _my_save_time;
			_my_computation_time = clock();

			// compute n generaions
			generateBoard();

			_my_computation_time = clock() - _my_computation_time;

			// save board to file --> footer
			saveFooter();
			/*}
			else
			{
				_my_computation_time = clock();
				generateBoard();
				_my_computation_time = clock() - _my_computation_time;

				std::cout<<"Computation time: "<<timeToString(_my_computation_time)<<std::endl;
			}*/
			if (_measure)
				std::cout << timeToString(_my_load_time + _my_computation_time + _my_save_time) << ";" << timeToString(_my_load_time) << ";" << timeToString(_my_computation_time) << ";" << timeToString(_my_save_time) << ";" << std::endl;
		}
	}
}

// compates measurement section of file with own measurement
void Application::compare()
{
	_my_load_time /*= _my_total_time*/ = clock();
	loadFile(_compare);
	if (!_measureSection)
		std::cout << "Measurement Sektion fehlt: " << _compare << std::endl;
	else
	{
		// if there is no save file store in temp file
		/*if(_saveFile=="")
			_saveFile="temp.gol";*/
		_my_load_time = clock() - _my_load_time;
		_my_save_time = clock();
		saveHeader();
		_my_save_time = clock() - _my_save_time;
		_my_computation_time = clock();
		generateBoard();
		_my_computation_time = clock() - _my_computation_time;
		//_my_total_time =  clock() - _my_total_time;
		saveFooter();
		// everything computed, now compate measurements
		std::cout<<"Measurement:"<<std::endl<<std::endl;
		std::cout<<_compare<<":"<<std::endl;
		std::cout<<"Total time:       "<<timeToString(_total_time)<<std::endl;
		std::cout<<"Load time:        "<<timeToString(_load_time)<<std::endl;
		std::cout<<"Save time:        "<<timeToString(_save_time)<<std::endl;
		std::cout<<"Computation time: "<<timeToString(_computation_time)<<std::endl<<std::endl;
		std::cout<<"Eigene Berechnung:"<<std::endl;
		std::cout<<"Total time:       "<<timeToString(_my_total_time)<<std::endl;
		std::cout<<"Load time:        "<<timeToString(_my_load_time)<<std::endl;
		std::cout<<"Save time:        "<<timeToString(_my_save_time)<<std::endl;
		std::cout<<"Computation time: "<<timeToString(_my_computation_time)<<std::endl;
		std::cout << timeToString((_my_load_time + _my_computation_time + _my_save_time) - (_load_time + _computation_time + _save_time)) << ";" << timeToString(_my_load_time - _load_time) << ";" << timeToString(_my_computation_time - _computation_time) << ";" << timeToString(_my_save_time - _save_time) << ";" << std::endl;
	}
}

// check if loaded file is valid
void Application::check()
{
	loadFile(_check);
	generateBoard();
	if ((*_gol) == (*_golEndState))
		std::cout << "Das Board ist korrekt" << std::endl;
	else
		std::cout << "Das Board ist falsch" << std::endl;
}

void Application::saveHeader()
{
	if (_saveFile != "")
	{
		std::ofstream oFile(_saveFile.c_str());

		if (oFile.is_open())
		{
			oFile << "COMPUTATION" << std::endl;
			oFile << _generations << std::endl;
			oFile << "BOARD" << std::endl;
			oFile << _gol->saveBoard();
			oFile << "END BOARD" << std::endl;
			oFile.close();
		}
	}
}

void Application::saveFooter()
{
	clock_t help = clock();

	if (_saveFile != "")
	{
		std::ofstream oFile(_saveFile.c_str(), std::ios::app);

		if (oFile.is_open())
		{
			oFile << "BOARD" << std::endl;
			oFile << _gol->saveBoard();
			oFile << "END BOARD" << std::endl;
			oFile << "END COMPUTATION" << std::endl;
			_my_save_time += clock() - help;
			_my_total_time = _my_load_time + _my_computation_time + _my_save_time;
			if (_measure)
			{
				oFile << "MEASUREMENT" << std::endl;
				oFile << timeToString(_my_total_time) << ";" << timeToString(_my_load_time) << ";" << timeToString(_my_computation_time) << ";" << timeToString(_my_save_time) << ";" << std::endl;
				oFile << "END MEASUREMENT" << std::endl;
			}
			oFile.close();
		}
	}
}

void Application::load()
{
	loadFile(_loadFile);
}

// creates a random board
void Application::createRandom()
{
	_gol = new GOL(_x, _y);
	_gol->randBoard(_seed);
}

// loads the file from disk and sets variables
void Application::loadFile(std::string file)
{
	std::ifstream iFile(file.c_str());
	std::string str;
	std::string board_start = "";
	std::string board_end = "";
	int next = 0;
	char c;
	int gen = 0;

	if (iFile.is_open())
	{
		while (iFile >> str)
		{
			std::stringstream sstream(str);
			switch (next)
			{
			case 0: if (str == "COMPUTATION")
				next = 1;
				if (str == "BOARD") {
					if (board_start == "")
						next = 2;
					else {
						if (board_end == "")
							next = 4;
						else
							next = 6;
					}
				}
				break;
			case 1: sstream >> gen;
				next = 0;
				break;
			case 2: sstream >> _x >> c >> _y;
				next = 3;
				break;
			case 3: if (str == "END")
				next = 0;
					else
				board_start += str;
				break;
			case 4: iFile >> str;
				next = 5;
				break;
			case 5: if (str == "END")
				next = 0;
					else
				board_end += str;
				break;
			case 6: if (str == "MEASUREMENT")
				next = 7;
				break;
			case 7: if (str == "END")
				next = 0;
					else
			{
				int h, m, s, ms;
				sstream >> h >> c >> m >> c >> s >> c >> ms >> c;
				_total_time = ms + s * 1000 + m * 60000 + h * 3600000;
				sstream >> h >> c >> m >> c >> s >> c >> ms >> c;
				_load_time = ms + s * 1000 + m * 60000 + h * 3600000;
				sstream >> h >> c >> m >> c >> s >> c >> ms >> c;
				_computation_time = ms + s * 1000 + m * 60000 + h * 3600000;
				sstream >> h >> c >> m >> c >> s >> c >> ms >> c;
				_save_time = ms + s * 1000 + m * 60000 + h * 3600000;
				_measureSection = true;
			}
					break;
			}
		}
		iFile.close();
	}

	// load generations from file if it was not set by param
	if (_generations == 0)
		_generations = gen;

	_gol = new GOL(_x, _y);
	_gol->loadBoard(board_start);

	if (_check != "" || _compare != "")
	{
		_golEndState = new GOL(_x, _y);
		_golEndState->loadBoard(board_end);
	}
}

void Application::generateBoard()
{
	for (int i = 0; i < _generations; ++i)
		_gol->evoluteBoard();
}

std::string Application::timeToString(clock_t timer)
{
	std::stringstream ss;
	bool negativ = false;
	if (timer < 0)
	{
		negativ = true;
		timer = abs(timer);
	}
	int ms = timer % 1000;
	timer = timer / 1000;
	// seconds
	time_t h, m, s;
	h = timer / 3600;
	m = (timer - (h * 3600)) / 60;
	s = (timer - (h * 3600)) - m * 60;

	if (negativ)
		ss << "-" << h << ":" << m << ":" << s << ":" << ms;
	else
		ss << h << ":" << m << ":" << s << ":" << ms;
	return ss.str();
}