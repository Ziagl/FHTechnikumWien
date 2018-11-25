#include "node.h"

Node::Node(void) : pred(0), g(0), h(0), f(0), nPred(0)
{
	p._x = 0;
	p._y = 0;
}

Node::Node(int _g, int _h, Node *_pred, int _nPred, Position2D _p)
{
	g = _g;
	h = _h;
	f = g + h;
	pred = _pred;
	nPred = _nPred;
	p = _p;
}

Node::~Node(void)
{
}
