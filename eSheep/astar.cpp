#include "astar.h"

AStar::AStar(Map *_map): start( 0, 0 ), goal( 0, 0 ), nTodo(0), nClosed(0), nodesExpanded(0)
{
	map=_map;
	todo  = new Node*[ LEVEL_WIDTH * LEVEL_HEIGHT ];
	closed = new Node*[ LEVEL_WIDTH * LEVEL_HEIGHT ];
}

AStar::AStar(Position2D &_start, Position2D &_goal, Map *_map) : start(_start), goal(_goal), nTodo(0), nClosed(0), nodesExpanded(0) 
{
	map=_map;
	todo  = new Node*[ LEVEL_WIDTH * LEVEL_HEIGHT ];
	closed = new Node*[ LEVEL_WIDTH * LEVEL_HEIGHT ];
	init(_start,_goal);
}

AStar::~AStar(void)
{
	for (int i = 0; i < nClosed; ++i)
		delete closed[ i ];
	for (int i = 0; i < nTodo; ++i)
		delete todo[ i ];

	delete todo;
	delete closed;
}

void AStar::init(Position2D &s, Position2D &g, bool enemy)
{
	_enemy=enemy;
	nTodo = nClosed = 0;
	Node *startNode = new Node();
	startNode->p = s;
	startNode->h = pathCostEstimate(s,g);
	startNode->f = startNode->h;
	startNode->pred = 0;
	startNode->nPred = 0;

	goal = g;

	pushNode(todo, &nTodo, startNode);
	lowestOpen = nTodo - 1;
	lowestCost = startNode->f;
}

int AStar::searchPath()
{
	// as long as there are nodes
	while(nTodo>0)
	{
		// choose best node
		Node *node = todo[lowestOpen];
		//clan::Console::write_line("New Node: %1,%2",node->p._x,node->p._y);
		removeNode((Node**)todo, &nTodo, lowestOpen);		// remove from todo list
		pushNode(closed, &nClosed, node);					// move to finalized list
		findLowestCost();

		if(node->p == goal)
		{
			// goal reached
			goalNode = node;
			return node->nPred + 1;
		}
		else
		{
			expandNode(node);								// add neighbour to todo list
			nodesExpanded++;
		}
	}
	// no path found!
	return -1;
}

void AStar::expandNode(Node *node)
{
	// special for hexagonal maps
	for ( int d = 0; d < NEIGHBOURS; d++ )
	{
		Position2D p = node->p.neighbour( d );

		// only if neighbour exists or it is no water
		if (isValid(p))
		{
			int newCost = node->g + getCost(p);

			Node *newNode = new Node(newCost, pathCostEstimate(p, goal),node, node->nPred+1, p );

			int io, ic, contained = 0, oldCost = -1;

			io = containsNode(todo, nTodo, newNode);
			ic = containsNode(closed, nClosed, newNode);

			if ( io != -1 || ic != -1 )
			{
				if ( io != -1 )
					oldCost = todo[ io ]->g; 
				else
					oldCost = closed[ ic ]->g;
			}

			if (oldCost != -1 && oldCost <= newCost)
			{
				delete newNode;
				continue;
			} else
			{
				if ( ic != -1 )
					removeNode(closed, &nClosed, ic);

				if ( io != -1 )
					removeNode( todo, &nTodo, io );

				pushNode( todo, &nTodo, newNode );
				findLowestCost();

			}
		}
	}
}

int AStar::getCost(Position2D &b)
{
	// cost for enemy without potentials
	//if(_enemy)
		return map->/*_fields[b._x][b._y].getValue()*/getValue(b._x,b._y);
	//else	// player costs with potentials from enemy
		//return map->_fields[b._x][b._y].getValue()*map->_potentials[b._x][b._y];
}

int AStar::pathCostEstimate(Position2D &s, Position2D &g)
{
	// cost estimation with Manhattan methode
	return abs(g._x - s._x) + abs(g._y - s._y);
}

void AStar::findLowestCost()
{
	lowestCost = 100000;
	for ( int i = 0; i < nTodo; i++ )
	{
		if ( todo[i]->f < lowestCost )
		{
			lowestCost = todo[ i ]->f;
			lowestOpen = i;
		}
	}
}

int AStar::getPath( Position2D *p )
{
	Position2D *dst = &p[goalNode->nPred];
	int length = goalNode->nPred + 1;
	Node *node = goalNode;

	while ( 1 ) 
	{
		*dst = node->p;
		*dst --;
		if ( node->pred != NULL )
			node = node->pred; else
			break;
	};

	return length;
}

void AStar::pushNode(Node **list, int *count, Node *node)
{
	list[(*count)++] = node;
}

int AStar::containsNode(Node **list, int count, Node *me)
{
	for(int i=0; i<count; ++i)
		if(list[i]->p == me->p)
			return i;
	return -1;
}

void AStar::removeNode(Node **list, int *count, int me)
{
	list[me]=list[--(*count)];
}

int AStar::isValid(Position2D &p)
{
	// check if this field is valid
	// special fields also are not valid --> 0...for water!
	if ( p._x < 0 || p._x >= LEVEL_WIDTH || p._y < 0 || p._y >= LEVEL_HEIGHT || map->/*_fields[p._x][p._y].getValue()*/getValue(p._x,p._y) == 0 )
		return 0;
	return 1;
}