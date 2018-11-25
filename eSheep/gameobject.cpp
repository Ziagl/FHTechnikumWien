#include "gameobject.h"

GameObject::GameObject(void)
{
}

GameObject::GameObject(clan::Canvas &canvas, clan::Image* tile, Position2D &pos)
{
	_canvas = canvas;
	_tile = tile;
	_pos = pos;
}

GameObject::~GameObject(void)
{
}

void GameObject::move(unsigned int time)
{
	
}

void GameObject::render()
{
	float scaleFactor;
#if SMALL==1
	scaleFactor=TILES_SCALE_FACTOR_SMALL;
#else
	scaleFactor=TILES_SCALE_FACTOR;
#endif
	// renders game object
	float _PPx = static_cast<float>((_pos._x*scaleFactor*TILES_SIZE_X)+((_pos._y%2==0)?TILES_OFFSET_X*scaleFactor:0));
	float _PPy = static_cast<float>((_pos._y*scaleFactor*TILES_OFFSET_Y));

	_tile->set_scale(scaleFactor,scaleFactor);
	_tile->draw(_canvas, _PPx, _PPy);
}