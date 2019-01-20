#pragma once

#include "bitboard.h"
#include <string>
#include <sstream>
#include <ctime>
#include <boost/random.hpp>
#include <boost/generator_iterator.hpp>

typedef boost::variate_generator<boost::mt19937, boost::uniform_int<>> v_gen;

class GOL
{
public:
	GOL(void);
	GOL(int x, int y);					// create board with dimension
	GOL(std::string file, int value);	// create board and load from file
	~GOL(void);

	void initBoard();					// initialize the board
	void randBoard(int seed);
	void loadBoard(std::string board);	// load board from a string
	std::string saveBoard();			// save board to a string

	void evoluteBoard();				// the evolution of a board to the next level
	bool operator==(GOL &other);

protected:
	BitBoard* _board1;
	BitBoard* _board2;
	bool _boardFlag;

private:
	void swapBoard();
};