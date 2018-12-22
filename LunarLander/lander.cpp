#include "Lander.h"

Lander::Lander(clan::Canvas &canvas)
{
	_canvas = canvas;;
	_engine = false;
	_landed = false;
	_save = true;
	_tilesVec.clear();
	_tilesVec.push_back(new clan::Image(canvas, ".\\Data\\Tiles\\lander_1.png"));
	_tilesVec.push_back(new clan::Image(canvas, ".\\Data\\Tiles\\lander_2.png"));
	_tilesVec.push_back(new clan::Image(canvas, ".\\Data\\Tiles\\lander_3.png"));
	_tilesVec.push_back(new clan::Image(canvas, ".\\Data\\Tiles\\lander_4.png"));
	_t = 0;
	_fuel = MAX_FUEL;
	registered = false;

	//Particle Zeugs
	setForce(0.0);
	_pos = Position2D(20.0f, 0.0f);
}

Lander::~Lander(void)
{
}

void Lander::move(unsigned int time)
{
	double scaled_time = time * TIME_SCALING;

	//gibt es Schub von dem Antrieb?
	if (_engine && _fuel > 0.0)
	{
		//Beschleunigung nach oben
		//_v1+=_dv1;
		_fuel -= scaled_time * FUEL_CONSUMPTION;
	}
	/*else
	{
		//freier Fall
		_v+=_dv;
	}
	//Beide Kräfte wirken auf Objekt "addieren"
	_speed = _v - _v1;*/
	_t += scaled_time;
	clan::Console::write_line("Force: %1 * Time: %2", getForce(), _t);
	//Kraft wirkt sich auch auf das Objekt aus (Geschwindigkeit)	== INTEGRATOR???
	integrate(time);

	if (_pos._y > LUNAR_SURFACE)
	{
		_pos._y = LUNAR_SURFACE;
		_landed = true;
		if ((_speed._y*INTERFACE_SPEED) > MAX_IMPACT_SPEED)
			_save = false;
	}

	setForce(0.0);
}

void Lander::render()
{
	clan::Image *image;
	if (_engine && _fuel > 0.0)
		image = _tilesVec[1];
	else
		image = _tilesVec[0];
	if (_landed)
	{
		if (!_save)
			image = _tilesVec[2];
		else
			image = _tilesVec[3];
	}
	image->set_scale(LANDER_SCALE_FACTOR, LANDER_SCALE_FACTOR);
	image->draw(_canvas, _pos._x, _pos._y);
}