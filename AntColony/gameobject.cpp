#include "gameobject.h"

GameObject::GameObject(void)
{
}

GameObject::GameObject(clan::Canvas &canvas, clan::Image* tile, clan::Image* pathtile, Position2D &pos)
{
	_canvas = canvas;
	_tile = tile;
	_pathtile = pathtile;
	_pos = pos;
	_path = 0;
	_pathlength = 0;
	_pathpos = 0;
	_newpathlength = 0;
	_newpath = 0;
	_timer = 0;
	pathchange = false;
}

GameObject::~GameObject(void)
{
}

void GameObject::deletePath()
{
	if (_path != 0)
		delete _path;
	_path = 0;
	_pathlength = 0;
	_pathpos = _pathlength;
}

void GameObject::setPath(Position2D *path, int pathlength)
{
	_path = path;
	_pathlength = pathlength;
	_pathpos = pathlength;
}

void GameObject::setNewPath(Position2D *path, int pathlength)
{
	_newpath = path;
	_newpathlength = pathlength;
}

void GameObject::move(unsigned int time)
{
	_timer += time;
	// only if there is a path!
	if (_path != 0 && _timer > MOVEMENT_PLAYER_UPDATE)
	{
		// set new path if exists
		if (_newpath != 0 && _newpathlength != 0)
		{
			//clan::Console::write_line("old path: start: %1,%2 next: %3,%4",_pos._x,_pos._y,_path[_pathpos-1]._x,_path[_pathpos-1]._y);
			deletePath();
			setPath(_newpath, _newpathlength);
			_newpath = 0;
			_newpathlength = 0;
			//clan::Console::write_line("new path: start: %1,%2 next: %3,%4",_pos._x,_pos._y,_path[_pathpos-1]._x,_path[_pathpos-1]._y);
		}
		else
		{
			_timer = 0;
		}
		if (_pathpos > 0)
		{
			// move character
			int x = _path[_pathpos - 1]._x - _pos._x;
			int y = _path[_pathpos - 1]._y - _pos._y;

			if (x > 0) _pos._x++;
			if (x < 0) _pos._x--;
			if (y > 0) _pos._y++;
			if (y < 0) _pos._y--;

			pathchange = false;

			if (_pos == _path[_pathpos - 1])
			{
				// reach goal -> do not render goal anymore
				_pathpos--;
				pathchange = true;
			}

			if (_pos == _path[0])
			{
				// reached next path field -> clear path
				delete[] _path;
				_path = 0;
				_pathlength = 0;
			}
		}
	}
}

void GameObject::render()
{
	float scaleFactor;
#if SMALL==1
	scaleFactor = TILES_SCALE_FACTOR_SMALL;
#else
	scaleFactor = TILES_SCALE_FACTOR;
#endif

	// render player
	float _PPx = static_cast<float>((_pos._x*scaleFactor*TILES_SIZE_X) + ((_pos._y % 2 == 0) ? TILES_OFFSET_X * scaleFactor : 0));
	float _PPy = static_cast<float>((_pos._y*scaleFactor*TILES_OFFSET_Y));
	// render objects
	// for moveable objects we need to add interpolation beteen points based on time!!
	if (_path != 0 && _pathpos > 0)
	{
		float _PPxNeu = static_cast<float>((_path[_pathpos - 1]._x*scaleFactor*TILES_SIZE_X) + ((_path[_pathpos - 1]._y % 2 == 0) ? TILES_OFFSET_X * scaleFactor : 0));
		float _PPyNeu = static_cast<float>((_path[_pathpos - 1]._y*scaleFactor*TILES_OFFSET_Y));

		float xdiff = (_PPxNeu - _PPx) / MOVEMENT_PLAYER_UPDATE;
		float ydiff = (_PPyNeu - _PPy) / MOVEMENT_PLAYER_UPDATE;

		_tile->set_scale(scaleFactor, scaleFactor);
		_tile->draw(_canvas, _PPx + (xdiff*_timer), _PPy + (ydiff*_timer));
	}
	else
	{
		_tile->set_scale(scaleFactor, scaleFactor);
		_tile->draw(_canvas, _PPx, _PPy);
	}
}
