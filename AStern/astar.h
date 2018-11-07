#pragma once

#include "position2d.h"
#include "node.h"
#include "map.h"

class AStar
{
public:
	AStar(Map *_map);
	AStar(Position2D &_start, Position2D &_goal, Map *_map);
	~AStar(void);

	void init(Position2D s, Position2D g, bool enemy = false); // init this algorithm
	int searchPath();						// find a path from start to goal
	void expandNode(Node *node);			// adds all neighbors to todo list
	int getCost(Position2D &b);				// returns cost for given field
	int pathCostEstimate(Position2D &s, Position2D &g);		// estimate costs
	void findLowestCost();					// finds element from todo list with minimal cost
	int getPath(Position2D *p);				// return complete path

	// node
	void pushNode(Node **list, int *count, Node *node);	// add node to list
	int containsNode(Node **list, int count, Node *me);	// list contains node already?
	void removeNode(Node **list, int *count, int me);	// remove node from list

private:
	int isValid(Position2D &p);				// check if field is valid (not outside map)

	Position2D start, goal;

	int lowestOpen;
	int lowestCost;
	int nodesExpanded;

	int nTodo, nClosed;
	Node **todo;					// todo list
	Node **closed;					// finalised list
	Node *goalNode;					// goald node (= start node - backtracking!)
	Map *map;

	bool _enemy;
};
