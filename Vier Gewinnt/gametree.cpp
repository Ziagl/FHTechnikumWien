#include "gametree.h"

GameTree::GameTree(Field* data, bool player, int deep)
{
	_deep = deep;

	_root = new GameNode(data, player == false ? RED : YELLOW, 0, deep);
	_root->createTree(deep);
}

GameTree::~GameTree(void)
{
	delete _root;
}

void GameTree::evaluateLeafes()
{
	_root->evaluateLeafes();
}

void GameTree::minimax()
{
	_root->minimax();
}

int GameTree::getTurn()
{
	if (_root->_child.size() > 0)
	{
		int x = _root->_child[0]->_x;
		int max = _root->_child[0]->_cost;
		for (std::vector<GameNode*>::iterator it = _root->_child.begin();
			it != _root->_child.end();
			++it)
			if (max < (*it)->_cost)
			{
				max = (*it)->_cost;
				x = (*it)->_x;
			}
		return x;
	}
	return -1;
}