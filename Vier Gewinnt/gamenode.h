#pragma once

#include "precomp.h"
#include "globals.h"
#include "field.h"

class GameNode
{
public:
	GameNode(Field* data, Fieldvalue color, int x, int deep);
	~GameNode(void);
	void createTree(int deep);
	void evaluateLeafes();
	void minimax();
	Fieldvalue negateColor(Fieldvalue color);

	std::vector<GameNode*> _child;		// child node
	Fieldvalue _color;					// color of minmax algorithm (different for each depth)
	int _cost;							// cost for turn
	bool _costset;
	Field* _data;						// node data
	int _x;
	int _deep;
private:
	void evaluate();					// sets cost
};
