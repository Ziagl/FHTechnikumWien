#include "gamenode.h"

GameNode::GameNode(Field* data, Fieldvalue color, int x, int deep)
{
	_costset = false;
	_child.clear();
	_data = data;
	_color = color;
	_x = x;
	_deep = deep;
}

GameNode::~GameNode(void)
{

}

void GameNode::createTree(int deep)
{
	if (deep > 0)
	{
		deep--;
		for (int i = 0; i < LEVEL_WIDTH; ++i)
		{
			Field* data = _data->getcopy();
			if (data->setField(i, negateColor(_color)))
			{
				// create new child node
				GameNode *node = new GameNode(data, negateColor(_color), i, deep);
				// create new children for childnode if not already won
				if (node->_data->getChain4(RED) == 0 && node->_data->getChain4(YELLOW) == 0)
					node->createTree(deep);
				// add node to list
				_child.push_back(node);
			}
		}

		/*bool winner = false;
		// is there a winner? --> if yes - do not create further nodes
		for(unsigned int i=0;i<_child.size(); ++i)
		{
			if(_child[i]->_data->getChain4(AI_COLOR)>0 || _child[i]->_data->getChain3(AI_COLOR)>1)
				winner=true;
			if(_child[i]->_data->getChain4(PLAYER_COLOR)>0 || _child[i]->_data->getChain3(PLAYER_COLOR)>1)
				winner=true;
		}

		// there is no winner --> create nodes
		if(winner!=true)
		{
			for(unsigned int i=0;i<_child.size(); ++i)
				_child[i]->createTree(deep);
		}*/
	}
}

void GameNode::evaluateLeafes()
{
	int count = 0;

	for (std::vector<GameNode*>::iterator it = _child.begin();
		it != _child.end();
		++it)
		(*it)->evaluateLeafes();

	if (_child.size() == 0)			// leaf!!!
		evaluate();
}

Fieldvalue GameNode::negateColor(Fieldvalue color)
{
	if (color == RED)
		return YELLOW;
	if (color == YELLOW)
		return RED;
	return color;
}

void GameNode::evaluate()
{
	int four = _data->getChain4(AI_COLOR);
	int three = _data->getChain3(AI_COLOR);
	int two = _data->getChain2(AI_COLOR);
	int empty = _data->getFree();

	int enemy_four = _data->getChain4(PLAYER_COLOR);
	int enemy_three = _data->getChain3(PLAYER_COLOR);
	int enemy_two = _data->getChain2(PLAYER_COLOR);

	//_cost = (four>0?WF1:0 + three*WF2 + two*WF3 + empty*WF4)/(TREE_DEEP-(_deep-1));
	//_cost -= (enemy_four>0?WFE1:0 + enemy_three*WFE2 + enemy_two*WFE3 + empty*WFE4)/(TREE_DEEP-(_deep-1));
	_cost = four > 0 ? WF1 : 0 + three * WF2 + two * WF3 /*+ empty*WF4*/;
	_cost -= enemy_four > 0 ? WFE1 : 0 + enemy_three * WFE2 + enemy_two * WFE3 /*+ empty*WFE4*/;
	_costset = true;
}

void GameNode::minimax()
{
	// if cost was not already set
	for (std::vector<GameNode*>::iterator it = _child.begin();
		it != _child.end();
		++it)
		if ((*it)->_costset != true)
			(*it)->minimax();
	// all children have a cost value
	_cost = _child[0]->_cost;
	_costset = true;
	if (_color == PLAYER_COLOR)
	{	// max
		for (std::vector<GameNode*>::iterator it = _child.begin();
			it != _child.end();
			++it)
			if ((*it)->_cost > _cost)
				_cost = (*it)->_cost;
	}
	else
	{	// min
		for (std::vector<GameNode*>::iterator it = _child.begin();
			it != _child.end();
			++it)
			if ((*it)->_cost < _cost)
				_cost = (*it)->_cost;
	}
}