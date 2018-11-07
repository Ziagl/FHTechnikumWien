#pragma once

#include "globals.h"
#include "position2d.h"

class GameObject
{
public:
	GameObject(void);
	GameObject(clan::Canvas &canvas, clan::Image* tile, clan::Image* pathtile, Position2D &pos);
	~GameObject(void);

	void setTile(clan::Image* tile) { _tile = tile; }
	clan::Image* getTile() { return _tile; }
	int getX() { return _pos._x; }
	int getY() { return _pos._y; }
	Position2D getPosition() { return _pos; }
	Position2D getNextPosition() { if (_path != 0) return _path[_pathpos]; else return Position2D(0, 0); }

	void deletePath();
	void setPath(Position2D *path, int pathlength);
	void setNewPath(Position2D *path, int pathlength);
	void render();
	void move(unsigned int time);

	bool pathchange;
protected:
	Position2D getHex(Position2D &pt);
	clan::Image* _tile;		// visualization
	clan::Image* _pathtile;	// path visualization (tile overlay)
	clan::Canvas _canvas;
	Position2D _pos;

	Position2D *_path;
	int _pathlength;
	Position2D *_newpath;
	int _newpathlength;
	int _pathpos;
	int _timer;
};
