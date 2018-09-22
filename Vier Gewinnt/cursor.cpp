#include "cursor.h"

CursorPos::CursorPos(clan::Canvas &canvas)
{
	_canvas = canvas;
	Position2D _pos(3, 0);
	_tilesVec.clear();
	_tilesVec.push_back(new clan::Image(_canvas, ".\\Data\\Tiles\\cursor_red.png"));
	_tilesVec.push_back(new clan::Image(_canvas, ".\\Data\\Tiles\\cursor_yellow.png"));
}

CursorPos::~CursorPos(void)
{
}

void CursorPos::render(bool player)
{
	clan::Image *image;
	if (player)
		image = _tilesVec[0];
	else
		image = _tilesVec[1];
	image->set_scale(CURSOR_SCALE_FACTOR, CURSOR_SCALE_FACTOR);
	image->draw(_canvas, static_cast<float>(_pos._x*CURSOR_SIZE) + (TILES_SIZE / 2 - CURSOR_OFFSET), static_cast<float>(_pos._y*CURSOR_SIZE));
}

void CursorPos::moveRight()
{
	if (_pos._x < LEVEL_WIDTH - 1)
		_pos._x++;
}

void CursorPos::moveLeft()
{
	if (_pos._x > 0)
		_pos._x--;
}

int CursorPos::getX()
{
	return _pos._x;
}