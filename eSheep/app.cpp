#include "app.h"

using namespace clan;

clan::ApplicationInstance<App> clanapp;

LOG_DECLARE;
LOG_USE();

App::App()
{
#if defined(WIN32) && !defined(__MINGW32__)
	clan::D3DTarget::set_current();
#else
	clan::OpenGLTarget::set_current();
#endif
	// set the window
	clan::DisplayWindowDescription desc;
	desc.set_title("eSheep");
	desc.set_size(clan::Sizef(WINDOW_WIDTH, WINDOW_HEIGHT), true);
	desc.set_allow_resize(true);

	window = clan::DisplayWindow(desc);
	canvas = clan::Canvas(window);

	// connect the window close event
	sc.connect(window.sig_window_close(), [&]() {_quit = true; });

	// connect a keyboard handler to on_key_up()
	sc.connect(window.get_keyboard().sig_key_up(), clan::bind_member(this, &App::on_keyboard));

	// connect a mouse handler (only for example it is not used)
	sc.connect(window.get_mouse().sig_key_down(), this, &App::on_mouse);
	sc.connect(window.get_mouse().sig_key_dblclk(), this, &App::on_mouse);

	_quit = false;
	srand(static_cast<int>(time(0)));	// initialize random number generator

	//Init Logging
	LOG_INIT("sheep.csv");
	LOG_MSG("Applikation gestartet");

	init();		//initialize

} // constructor

App::~App(void) {
	LOG_MSG("Applikation beendet");
} // destructor

bool App::update()
{
	long time_loopstart = clan::System::get_time();

	//rendern
	canvas.clear(clan::Colorf(0.0f, 0.0f, 0.0f));
	render();

	if (_timer > TICKTIMER && _pause == false)
	{
		compute();
		_timer = 0;
	}

	window.flip();
	long time_loopend = clan::System::get_time();
	_timer += time_loopend - time_loopstart;
	
	return !_quit;
}


void App::on_keyboard(const clan::InputEvent &key)
{
	switch (key.id) {
	case clan::keycode_escape:
	case clan::keycode_q:
		_quit = true; break;
	case clan::keycode_space:
		_pause = !_pause; break;
	}
}

void App::on_mouse(const clan::InputEvent &key) {
	// mouse position
	const float x = key.mouse_pos.x;
	const float y = key.mouse_pos.y;
	Position2D pos(x, y);

	// mouse action
	switch (key.id)
	{
	case mouse_left:
		createSheep(pos);
		break;
	case mouse_right:
		_map->changeTile(getHex(pos));
		break;
	}
}

void App::init()
{
	_pause = false;
	_round = 0;
	_grascounter = 0;
	_map = new Map(canvas);
} // init

void App::render(void)
{
	// render map
	_map->render();

	// render sheeps
	for (std::vector<Sheep>::iterator it = _sheeps.begin();
		it != _sheeps.end();
		++it)
		(*it).render(_map);

	// render HUD
	std::stringstream str;
	str << "Runde: " << _round;
	std::string s = str.str();
	_map->_font->draw_text(canvas, 2.0f, 10.0f, s, clan::Colorf::red);
} // render

void App::compute()
{
	_round++;

	// write log
	char s[200];
	sprintf(s, "starte Runde %d", _round);
	LOG_MSG(s);

	// grow plants (every 5th step)
	_grascounter++;
	if (_grascounter == GRASTIMER)
	{
		_grascounter = 0;
		_map->growGras();
	}
	_map->resetSheep();

	// update map
	// sheeps to map
	for (std::vector<Sheep>::iterator it = _sheeps.begin();
		it != _sheeps.end();
		++it)
		if (it->_sheepAlive)
			_map->setSheep(it->getPosition());

	std::vector<Sheep> tempchildren;

	// update sheeps
	for (std::vector<Sheep>::iterator it = _sheeps.begin();
		it != _sheeps.end();
		++it)
	{
		(*it).mychildren.clear();
		(*it).compute(_map, _sheeps);
		if ((*it).mychildren.size() > 0)	// if there are children
			for (std::vector<Sheep>::iterator it2 = (*it).mychildren.begin();
				it2 != (*it).mychildren.end();
				++it2)
				tempchildren.push_back(*it2);
	}

	// new children?
	if (tempchildren.size() > 0)
		for (std::vector<Sheep>::iterator it = tempchildren.begin();
			it != tempchildren.end();
			++it)
			_sheeps.push_back(*it);
} // compute

void App::createSheep(Position2D &pos)
{
	Position2D p = getHex(pos);
	if (_map->getValue(p._x, p._y) > 0)
	{
		int number = _sheeps.size();
		_sheeps.push_back(Sheep(canvas, _map->_tilesMap["sheep0"], p, number));
	}
}

// convert pixel coordinates into hexagonal map coordinate system
Position2D App::getHex(Position2D &pt)
{
	// precalculate the 'hotspot' regions
	float scale_factor;
#if SMALL==0
	scale_factor = TILES_SCALE_FACTOR;
#else
	scale_factor = TILES_SCALE_FACTOR_SMALL;
#endif
	const float MAPTILE_HEIGHT_HOT = TILES_SIZE_Y * scale_factor * .77F;
	const float MAPTILE_UPPERTHIRD = MAPTILE_HEIGHT_HOT * .23F;
	const float ANGLE = 0.589F;		//25°
	const float ANGLE2 = 1.8095F;	//155°
	const float MAPTILE_LEFTHALF = (TILES_SIZE_X * scale_factor) / 2;

	int row = (int)((pt._y + (pt._y / MAPTILE_HEIGHT_HOT)) / MAPTILE_HEIGHT_HOT);
	int col = (int)((pt._x + (pt._x / (TILES_SIZE_X * scale_factor))) / (TILES_SIZE_X * scale_factor));

	// convert the point (mouse coord) to coordinates relative to the current square
	pt._x -= (int)(col * (TILES_SIZE_X * scale_factor)) /*- col*/;
	pt._y -= (int)(row * MAPTILE_HEIGHT_HOT) - row;

	if (pt._y < MAPTILE_UPPERTHIRD) {
		if (row % 2 == 0) {
			if (pt._x < MAPTILE_LEFTHALF && pt._x * ANGLE > pt._y)
				row--;
			else if (pt._x >= MAPTILE_LEFTHALF && (pt._x - MAPTILE_LEFTHALF) / ANGLE2 < (MAPTILE_UPPERTHIRD - pt._y))
				row--;
			else if (pt._x < MAPTILE_LEFTHALF)
				col--;
		}
		else {
			if (pt._x >= MAPTILE_LEFTHALF && (pt._x - MAPTILE_LEFTHALF) * ANGLE > pt._y)
				row--;
			else if (pt._x < MAPTILE_LEFTHALF && (pt._x / ANGLE2 < (MAPTILE_UPPERTHIRD - pt._y))) {
				row--;
				col--;
			}
		}
	}
	else if (row % 2 == 0 && pt._x < MAPTILE_LEFTHALF) {
		// if the point of interest is within the lower 2/3 of the hex, then there are
		// two possibilities - even rows (0, 2, 4, ...) contain two hexes, whereas odd
		// rows (1, 3, 5, ...) have only one hex.
		col--;
	}

	if (col < 0 || row < 0 || row >= LEVEL_HEIGHT || col >= ((row % 2 == 0) ? LEVEL_WIDTH : LEVEL_WIDTH + 1) || pt._x < 0)
		return Position2D(-1, -1);
	else
		return Position2D(col, row);
}
