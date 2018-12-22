#include "BgObject.h"

BgObject::BgObject(clan::Canvas &canvas, std::string image_name, float x, float y)
{
	_canvas = canvas;
	_pos._x = x;
	_pos._y = y;
	_image = new clan::Image(canvas, image_name);
}

BgObject::~BgObject(void)
{
}

void BgObject::render()
{
	_image->set_scale(LANDER_SCALE_FACTOR, LANDER_SCALE_FACTOR);
	_image->draw(_canvas, static_cast<float>(_pos._x), static_cast<float>(_pos._y));
}

void BgObject::render(float scale)
{
	_image->set_scale(scale, scale);
	_image->draw(_canvas, static_cast<float>(_pos._x), static_cast<float>(_pos._y));
}