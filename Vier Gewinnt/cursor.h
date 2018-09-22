#pragma once

#include "precomp.h"
#include "globals.h"
#include "position2D.h"

class CursorPos
{
public:
	CursorPos(clan::Canvas &canvas);
	~CursorPos(void);
	void moveRight();
	void moveLeft();
	int getX();
	void render(bool player);		// render cursor
private:
	clan::Canvas _canvas;
	std::vector<clan::Image*> _tilesVec;
	Position2D _pos;
};
