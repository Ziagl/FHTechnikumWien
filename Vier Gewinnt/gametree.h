#pragma once

#include "globals.h"
#include "gamenode.h"

class GameTree
{
public:
	GameTree(Field* data, bool player, int deep);
	~GameTree(void);
	void evaluateLeafes();
	void minimax();
	int getTurn();
private:
	int _deep;			// max. deep
	GameNode* _root;	// root
};