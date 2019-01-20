#include "gol.h"

GOL::GOL(void)
{
}

GOL::GOL(int x, int y)
{
	_boardFlag = true;
	_board1 = new BitBoard(x, y);
	_board2 = new BitBoard(x, y);
}

GOL::~GOL(void)
{
#ifndef RELEASE
	assert(_board1);
#endif
	if (_board1 != 0) delete _board1;
#ifndef RELEASE
	assert(_board2);
#endif
	if (_board2 != 0) delete _board2;
}

void GOL::randBoard(int seed)
{
	boost::mt19937 _gen(seed);
	boost::uniform_int<> dist(0, 1);
	v_gen vargen(_gen, dist);
	boost::generator_iterator<v_gen> zahl(&vargen);

	int k = 0;
	for (int i = 0; i < _board1->getWidth(); ++i)
		for (int j = 0; j < _board1->getHeight(); ++j)
		{
			k = *zahl++;
			if (_boardFlag)
				_board1->set(i, j, k == 0);
			else
				_board2->set(i, j, k == 0);
		}

	swapBoard();
}

void GOL::evoluteBoard()
{
	BitBoard* in;
	BitBoard* out;

	if (_boardFlag)
	{
		in = _board2;
		out = _board1;
	}
	else
	{
		in = _board1;
		out = _board2;
	}


	for (int i = 0; i < in->getWidth(); ++i)
		for (int j = 0; j < in->getHeight(); ++j)
			out->set(i, j, in->stayAlive(i, j));

	swapBoard();
}

void GOL::swapBoard()
{
	_boardFlag = !_boardFlag;
}

void GOL::loadBoard(std::string str)
{
	BitBoard* b;
	int count = 0;
	if (_boardFlag)
		b = _board1;
	else
		b = _board2;

	for (int i = 0; i < b->getWidth(); ++i)
		for (int j = 0; j < b->getHeight(); ++j)
		{
			b->set(i, j, str.at(count) == 'x' ? true : false);
			++count;
		}
	swapBoard();
}

std::string GOL::saveBoard()
{
	BitBoard* b;
	if (_boardFlag)
		b = _board2;
	else
		b = _board1;
	std::stringstream sstr;
	sstr << b->getWidth() << "," << b->getHeight() << "\n";

	for (int i = 0; i < b->getWidth(); ++i)
	{
		for (int j = 0; j < b->getHeight(); ++j)
		{
			if (b->get(i, j))
				sstr << "x";
			else
				sstr << ".";
		}
		sstr << "\n";
	}
	return sstr.str();
}

bool GOL::operator ==(GOL &other)
{
	BitBoard* b1;
	if (_boardFlag)
		b1 = _board2;
	else
		b1 = _board1;
	BitBoard* b2;
	if (other._boardFlag)
		b2 = other._board2;
	else
		b2 = other._board1;

	return (*b1) == (*b2);
}