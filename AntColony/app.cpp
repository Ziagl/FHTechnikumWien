#include "app.h"

using namespace clan;

clan::ApplicationInstance<App> clanapp;

App::App()
{
#if defined(WIN32) && !defined(__MINGW32__)
	clan::D3DTarget::set_current();
#else
	clan::OpenGLTarget::set_current();
#endif

	// set the window
	clan::DisplayWindowDescription desc;
	desc.set_title("AntColony");
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

	init();		//initialize
}

bool App::update()
{
	compute();

	if (_renderall)
	{
		// clear display with white background
		canvas.clear(clan::Colorf(0.0f, 0.0f, 0.0f));

		render();

		window.flip(1);
	}

	return !_quit;
}


// keyboard key was pressed
void App::on_keyboard(const clan::InputEvent &key)
{
	switch (key.id) {
	case clan::keycode_escape:
	case clan::keycode_q:
		_quit = true; break;
	//case clan::keycode_space:
	//	compute(); break;
	}
}

// mouse key was pressed
void App::on_mouse(const clan::InputEvent &key) {
	// mouse position
	const float x = key.mouse_pos.x;
	const float y = key.mouse_pos.y;

	// mouse action
	switch (key.id)
	{
	case mouse_left:
		// check if the mouse click occured inside the application window
		if (x >= 0 && x <= WINDOW_WIDTH &&
			y >= 0 && y <= WINDOW_HEIGHT)
		{
			Position2D pos((int) x, (int) y);
			if (_start == 0)
			{
				insertStart(pos);
			}
			else
			{
				if (_ziel == 0)
				{
					insertZiel(pos);
					_renderall = false;
				}
				else
				{
					// both positions set, change world
					_map->changeTile(getHex(pos));
				}
			}
		} // if - mouse click occured inside app window
		break;
	case mouse_right:
		break;
	case mouse_middle:
		break;
	case mouse_wheel_up:
		break;
	case mouse_wheel_down:
		break;
		//nothing
	}
}

void App::init(void)
{
	_map = new Map(canvas);
	_ziel = _start = 0;
	_renderall = true;
	_anthill = 0;
	_antCounter = 0;
} // init

void App::render(void)
{
	_map->render();
	if (_start != 0)
		_start->render();
	if (_ziel != 0)
		_ziel->render();
} // render

void App::compute()
{
	if (_renderall == false)
	{
		_start->move(_map, _ziel->getPosition());
		// if ant reached goal -> kill
		if (_start->_reachedGoal)
		{
			// create new ant
			_start->reset();
			_start->_pos._x = _anthill->_x;
			_start->_pos._y = _anthill->_y;
			_antCounter++;

			clan::Console::write_line("Ants: %1", _antCounter);

			// render frame
			canvas.clear(clan::Colorf(0.0f, 0.0f, 0.0f));
			render();
			window.flip();
		}
	}
} // compute

void App::insertStart(Position2D pos)
{
	Position2D p = getHex(pos);
	if (_map->_fields[p._x][p._y].getValue() > 0)
	{
		_start = new Ant(canvas, _map->_tilesMap["start"], _map->_tilesMap["path"], p);
		_anthill = new Position2D(p._x, p._y);
		_antCounter++;
	}
}

void App::insertZiel(Position2D pos)
{
	Position2D p = getHex(pos);
	if (_map->_fields[p._x][p._y].getValue() > 0)
		_ziel = new GameObject(canvas, _map->_tilesMap["target"], _map->_tilesMap["path"], p);
}

// convert pixel coordinates into hexagonal map coordinate system
Position2D App::getHex(Position2D pt)
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
