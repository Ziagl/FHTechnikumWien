#pragma once

#include "BitBoard.h"
#include <string>
#include <sstream>
#include <ctime>
#include <boost/random.hpp>
#include <boost/generator_iterator.hpp>
#include <omp.h>

typedef boost::variate_generator<boost::mt19937, boost::uniform_int<>> v_gen;

class GOL
{
public:
	GOL(void);
	GOL(int x, int y);					//create Board with dimension
	GOL(std::string file, int value);	//create Board and load from file
	~GOL(void);

	void initBoard();					//initialize the Board
	void randBoard(int seed);
	void loadBoard(std::string board);	//load Board from a string
	std::string saveBoard();			//save Board to a string

	void evoluteBoard();				//the evolution of a Board to the next level
	bool operator==(GOL &other);

protected:
	BitBoard* _board1;
	BitBoard* _board2;
	bool _boardFlag;

private:
	void swapBoard();
};