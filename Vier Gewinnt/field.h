#pragma once

#include "globals.h"
#include "position2D.h"

class Field
{
public:
	Field(void);
	~Field(void);

	// get
	Fieldvalue getField(int x, int y);
	Fieldvalue getField(Position2D pos);
	// set
	void setField(int x, int y, Fieldvalue value);
	void setField(Position2D pos, Fieldvalue value);
	bool setField(int x, bool player);		// sets the field for the player, returns true if it worked (false if there is no more space in this cell)
	bool setField(int x, Fieldvalue color);
	// copy
	Field* getcopy();
	// counts
	int getChain4(Fieldvalue player);
	int getChain3(Fieldvalue player);
	int getChain2(Fieldvalue player);		// get a chain of 2,3,4 elements
	int getFree();

	void empty();							// clear field
private:
	void updateCounts(int x, int y, Fieldvalue color);
	void check(int x, int y, int i, int j, Fieldvalue color);
	void updateCounter(int v1, int v2, Fieldvalue color);
	int _help;
protected:
	unsigned char _field[LEVEL_WIDTH][LEVEL_HEIGHT];		// 2D map of board
	int _count4[2];
	int _count3[2];
	int _count2[2];
	int _free;
};
