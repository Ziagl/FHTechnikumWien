#include "Field.h"
#include <string.h>

Field::Field(void)
{
	// set all fields empty
	/*for(int i=0; i<LEVEL_HEIGHT; ++i)
		for(int j=0; j<LEVEL_WIDTH; ++j)
			_field[j][i] = EMPTY;*/
	_free = LEVEL_HEIGHT * LEVEL_WIDTH;
	_count2[0] = 0;
	_count2[1] = 0;
	_count3[0] = 0;
	_count3[1] = 0;
	_count4[0] = 0;
	_count4[1] = 0;
	/*_field = new unsigned char[LEVEL_WIDTH];
	for(int i=0; i<LEVEL_WIDTH;++i)
		_field[i] = new unsigned char [LEVEL_HEIGHT];*/
}

Field::~Field(void)
{
}

void Field::empty()
{
	for (int i = 0; i < LEVEL_HEIGHT; ++i)
		for (int j = 0; j < LEVEL_WIDTH; ++j)
			_field[j][i] = EMPTY;
}

Fieldvalue Field::getField(int x, int y)
{
	if (_field[x][y] == EMPTY)
		return EMPTY;
	if (_field[x][y] == RED)
		return RED;

	return YELLOW;
}

Fieldvalue Field::getField(Position2D pos)
{
	if (_field[pos._x][pos._y] == EMPTY)
		return EMPTY;
	if (_field[pos._x][pos._y] == RED)
		return RED;

	return YELLOW;
}

void Field::setField(int x, int y, Fieldvalue value)
{
	_field[x][y] = value;
}

void Field::setField(Position2D pos, Fieldvalue value)
{
	_field[pos._x][pos._y] = value;
}

bool Field::setField(int x, bool player)
{
	for (int i = LEVEL_HEIGHT - 1; i >= 0; --i)
	{
		if (_field[x][i] == EMPTY)
		{
			if (player)
			{
				_field[x][i] = RED;
				updateCounts(x, i, RED);
			}
			else
			{
				_field[x][i] = YELLOW;
				updateCounts(x, i, YELLOW);
			}
			return true;
		}
	}

	// no empty field left
	return false;
}

bool Field::setField(int x, Fieldvalue color)
{
	for (int i = LEVEL_HEIGHT - 1; i >= 0; --i)
	{
		if (_field[x][i] == EMPTY)
		{
			_field[x][i] = color;
			updateCounts(x, i, color);
			return true;
		}
	}

	// no empty field left
	return false;
}

Field* Field::getcopy()
{
	Field* ret = new Field();

	memcpy(ret->_field, this->_field, LEVEL_WIDTH*LEVEL_HEIGHT);
	ret->_count2[0] = this->_count2[0];
	ret->_count2[1] = this->_count2[1];
	ret->_count3[0] = this->_count3[0];
	ret->_count3[1] = this->_count3[1];
	ret->_count4[0] = this->_count4[0];
	ret->_count4[1] = this->_count4[1];
	ret->_free = this->_free;

	return ret;
}

int Field::getChain4(Fieldvalue player)
{
	return _count4[player - 1];
}

int Field::getChain3(Fieldvalue player)
{
	return _count3[player - 1];
}

int Field::getChain2(Fieldvalue player)
{
	return _count2[player - 1];
}

int Field::getFree()
{
	return _free;
}

void Field::updateCounts(int x, int y, Fieldvalue color)
{
	_free--;

	int n[8] = { 0 };
	_help = 0;
	check(x, y, -1, -1, color);
	n[0] = _help;
	_help = 0;
	check(x, y, 0, -1, color);
	n[1] = _help;
	_help = 0;
	check(x, y, 1, -1, color);
	n[2] = _help;
	_help = 0;
	check(x, y, -1, 0, color);
	n[3] = _help;
	_help = 0;
	check(x, y, 1, 0, color);
	n[4] = _help;
	_help = 0;
	check(x, y, -1, 1, color);
	n[5] = _help;
	_help = 0;
	check(x, y, 0, 1, color);
	n[6] = _help;
	_help = 0;
	check(x, y, 1, 1, color);
	n[7] = _help;

	// evaluate surrounding
	// horizontal
	updateCounter(n[3], n[4], color);
	// vertical
	updateCounter(n[1], n[6], color);
	// diagonal
	updateCounter(n[0], n[7], color);
	updateCounter(n[5], n[2], color);
}

void Field::updateCounter(int v1, int v2, Fieldvalue color)
{
	// 4
	if (v1 + v2 + 1 >= 4)
	{
		_count4[color - 1]++;
		//  *x** or ***x
		if (v1 == 2 || v2 == 2)
			_count2[color - 1]--;
		if (v1 == 3 || v2 == 3)
			_count3[color - 1]--;
	}
	else
	{
		// 3
		if (v1 + v2 + 1 == 3)
		{
			_count3[color - 1]++;
			// *x* or **x
			if (v1 == 2 || v2 == 2)
				_count2[color - 1]--;
		}
		else
		{
			// 2
			if (v1 + v2 + 1 == 2)
				_count2[color - 1]++;
		}
	}
}

void Field::check(int x, int y, int i, int j, Fieldvalue color)
{
	if (!(x + i<0 || y + j<0 || x + i>LEVEL_WIDTH - 1 || y + j>LEVEL_HEIGHT - 1))
		if (_field[x + i][y + j] == color)
		{
			_help++;
			check(x + i, y + j, i, j, color);
		}
}