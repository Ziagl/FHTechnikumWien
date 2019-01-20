#include "bitboard.h"

BitBoard::BitBoard(void)
{
}

BitBoard::BitBoard(int width, int height)
{
	_width = width;
	_height = height;
	_bits.resize(width*height);
	_bits.empty();
}

BitBoard::~BitBoard(void)
{
	_bits.reset();
}

bool BitBoard::get(int x, int y)
{
	if (x<0 || x>_width - 1 || y<0 || y>_height - 1)
		prepareCoordinates(x, y);
	return _bits[x*_width + y] == 0;
}

void BitBoard::set(int x, int y, bool value)
{
	if (x<0 || x>_width - 1 || y<0 || y>_height - 1)
		prepareCoordinates(x, y);

	if (value)
		_bits[x*_width + y] = 0;
	else
		_bits[x*_width + y] = 1;
}

int BitBoard::getWidth()
{
	return _width;
}

int BitBoard::getHeight()
{
	return _height;
}

void BitBoard::setWidth(int width)
{
	_width = width;
}

void BitBoard::setHeight(int height)
{
	_height = height;
}

void BitBoard::prepareCoordinates(int &x, int &y)
{
	int i;
	i = x;
	while (i < 0)
	{
		i = i + _width;
	}
	x = i % _width;
	i = y;
	while (i < 0)
	{
		i = i + _height;
	}
	y = i % _height;
}

bool BitBoard::operator ==(BitBoard &other)
{
	// stop if other board has different dimensions
	if (_width != other.getWidth() || _height != other.getHeight())
		return false;

	// if first bit is not the same -> boards are different
	for (int i = 0; i < _width; i++)
		for (int j = 0; j < _height; j++) {
			bool first = (_bits[i*_width + j] == 0);
			bool second = other.get(i, j);
			if (first != second)
				return false;
		}

	// equal boards
	return true;
}

bool BitBoard::stayAlive(int x, int y)
{
	if (x<0 || x>_width - 1 || y<0 || y>_height - 1)
		prepareCoordinates(x, y);
	int count = 0;

	// count neighbours
	count += get(x - 1, y - 1) ? 1 : 0;
	count += get(x, y - 1) ? 1 : 0;
	count += get(x + 1, y - 1) ? 1 : 0;
	count += get(x - 1, y) ? 1 : 0;
	count += get(x + 1, y) ? 1 : 0;
	count += get(x - 1, y + 1) ? 1 : 0;
	count += get(x, y + 1) ? 1 : 0;
	count += get(x + 1, y + 1) ? 1 : 0;

	if (get(x, y) == false)
	{
		if (count == 3)
			return true;
		else
		{
			return false;
		}
	}
	else
	{
		if (count >= 2 && count <= 3)
			return true;
		else
			return false;
	}

	return get(x, y);
}