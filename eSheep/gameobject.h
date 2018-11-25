#pragma once

#include "globals.h"
#include "position2D.h"

class GameObject
{
public:
	GameObject(void);
	GameObject(clan::Canvas &canvas, clan::Image* tile, Position2D &pos);
	~GameObject(void);
	void setTile(clan::Image* tile) {_tile=tile;}
	clan::Image* getTile() {return _tile;}
	int getX() { return _pos._x;}
	int getY() { return _pos._y;}
	Position2D getPosition() { return _pos;}

	void render();
	void move(unsigned int time);

protected:
	clan::Image* _tile;
	clan::Canvas _canvas;

	Position2D _pos;
};
