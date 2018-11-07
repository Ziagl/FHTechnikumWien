#pragma once

#include "position2d.h"

class Node
{
	friend class AStar;
public:
	Node(void);
	Node(int _g, int _h, Node *_pred, int _nPred, Position2D _p);
	~Node(void);
private:
	int g, h, f;
	Node *pred;
	int nPred;
	Position2D p;
};
