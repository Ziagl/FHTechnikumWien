#pragma once

#include "precomp.h"
#include "globals.h"
#include "position2d.h"

class BgObject
{
public:
	BgObject() {};
	BgObject(clan::Canvas &canvas, std::string image_name, float x, float y);
	~BgObject(void);
	void render();					// renders object
	void render(float scale);
private:
	clan::Canvas _canvas;
	clan::Image* _image;
	Position2D _pos;
};