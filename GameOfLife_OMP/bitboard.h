#pragma once

#include <boost/dynamic_bitset.hpp>

class BitBoard
{
public:
	BitBoard(void);
	BitBoard(int width, int height);
	~BitBoard(void);

	bool get(int x, int y);
	void set(int x, int y, bool value);

	int getWidth();
	int getHeight();
	void setWidth(int width);
	void setHeight(int height);

	bool operator==(BitBoard &other);

	bool stayAlive(int x, int y);
private:
	void prepareCoordinates(int &x, int &y);

	int _width, _height;
	boost::dynamic_bitset<> _bits;
};