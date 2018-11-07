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
	desc.set_title("AStern");
#if SMALL == 1
	desc.set_size(clan::Sizef(WINDOW_SIZE_SMALL_X, WINDOW_SIZE_SMALL_Y), true);
#else
	desc.set_size(clan::Sizef(WINDOW_SIZE_X, WINDOW_SIZE_Y), true);
#endif
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

App::~App() {
	delete _map;
	delete _pathfinder;
	delete _enemy1;
	delete _enemy2;
}

bool App::update()
{
	compute();

	// clear display with black background
	canvas.clear(clan::Colorf(0.0f, 0.0f, 0.0f));

	render();

	window.flip(1);

	return !_quit;
}

void App::init(void)
{
	_start = 0;
	_ziel = 0;
	_enemy1 = 0;
	_enemy2 = 0;
	_Estart._x = -1;
	_Eziel._x = -1;

	// initialize map an astar object
	_map = new Map(canvas);
	_pathfinder = new AStar(_map);
	// create enemies
	_Estart = Position2D((int)LEVEL_WIDTH / 2, 0);
	_Eziel = Position2D((int)LEVEL_WIDTH / 2, LEVEL_HEIGHT - 1);
	_enemy1 = new EnemyObject(canvas, _map->_tilesMap["enemy"], _map->_tilesMap["path_enemy1"], _Eziel);
	_enemy1->setWaypoints(_Estart, _Eziel);
	_enemy1->_agression = ENEMY_AGGRESSION;
	createPath(_enemy1);
	_Estart = Position2D(0, (int)LEVEL_HEIGHT / 2);
	_Eziel = Position2D(LEVEL_WIDTH - 1, (int)LEVEL_HEIGHT / 2);
	_enemy2 = new EnemyObject(canvas, _map->_tilesMap["enemy"], _map->_tilesMap["path_enemy2"], _Eziel);
	_enemy2->setWaypoints(_Estart, _Eziel);
	_enemy2->_agression = ENEMY_AGGRESSION;
	createPath(_enemy2);

	_player = true;			// player 1
	_victory = false;

	_time = 0;

	time_loopstart = 0;
	time_loopend = 0;
	time_elapsed = 0;
	time_pathupdater = 0;
} // init

void App::render(void)
{
	time_loopstart = clan::System::get_time();

	_map->render();
	// render enemies
	if (_enemy1 != 0)
	{
		// reached goal --> create new path
		if (_enemy1->hasPath() == false)
		{
			createPath(_enemy1);
		}
		_enemy1->move(time_elapsed / 2);
		_enemy1->render();
	}
	if (_enemy2 != 0)
	{
		if (_enemy2->hasPath() == false)
		{
			createPath(_enemy2);
		}
		_enemy2->move(time_elapsed / 2);
		_enemy2->render();
	}

	if (_start != 0)
	{
		_start->move(time_elapsed);
		if (_start->pathchange && time_pathupdater > PATH_UPDATE_TIME && _ziel != 0)
		{
			updatePath();
			time_pathupdater = 0;
			_start->pathchange = false;
		}
		_start->render();
	}
	if (_ziel != 0)
	{
		_ziel->render();
		// if start is goal, remove goal
		if (_start->getPosition() == _ziel->getPosition())
		{
			delete _ziel;
			_ziel = 0;
		}
	}

	// compute time based rendering
	window.flip(0);

	time_loopend = clan::System::get_time();
	time_elapsed = time_loopend - time_loopstart;
	time_pathupdater += time_elapsed;
} // render

void App::compute()
{
} // compute

/////////////////////////////////////////////////////////////////////////////
// world implementation:

// keyboard key was pressed
void App::on_keyboard(const clan::InputEvent &key)
{
	switch (key.id) {
		case clan::keycode_escape:
		case clan::keycode_q:
			_quit = true; break;
		case clan::keycode_enter:
			deletePoints(); break;
	}
}

// mouse key was pressed
void App::on_mouse(const clan::InputEvent &key) {
	// mouse position
	const float x = key.mouse_pos.x;
	const float y = key.mouse_pos.y;

	// place game objects with left mouse key
	// check if the mouse click occured inside the application window
#if SMALL == 1
	if (x >= 0 && x <= WINDOW_SIZE_SMALL_X && y >= 0 && y <= WINDOW_SIZE_SMALL_Y)
#else
	if (x >= 0 && x <= WINDOW_SIZE_X && y >= 0 && y <= WINDOW_SIZE_Y)
#endif
	{
		// mouse action
		switch (key.id)
		{
			case mouse_left:
				// set start and goal
				if (_start == 0)		// there is no start -> create one
					insertStart(Position2D((int) x, (int) y));
				else
				{
					if (_ziel == 0)		// there is no goal
					{
						insertZiel(Position2D((int) x, (int) y));	// -> create one

						if (_ziel != 0)
						{
							_map->clearPotentials();			// clear potentials
							// add enemies to potential map
							if (_enemy1 != 0)
								_map->setPotentials(_enemy1->getPosition(), _enemy1->_agression, POTENTIAL_SIZE);
							if (_enemy2 != 0)
								_map->setPotentials(_enemy2->getPosition(), _enemy2->_agression, POTENTIAL_SIZE);
							// calculate path
							_pathfinder->init(_ziel->getPosition(), _start->getPosition());
							int length = _pathfinder->searchPath();
							if (length < 0)
							{
								// no path found -> print error message
								clan::Console::write_line("Spieler: Keinen Weg gefunden.");
							}
							else
							{
								Position2D *path = new Position2D[length];
								_pathfinder->getPath(path);
								_start->setPath(path, length);
							}
						}
					}
				}
				break;
			case mouse_right:
				// add enemies with right mouse button
				/*if(_Eziel._x==-1)
				{
					Position2D p = getHex(Position2D(in_key.mouse_pos.x,in_key.mouse_pos.y));
					if(_map->_fields[p._x][p._y].getValue()!=0)
						_Eziel = p;
				}
				else
				{
					if(_Estart._x==-1)
					{
						Position2D p = getHex(Position2D(in_key.mouse_pos.x,in_key.mouse_pos.y));
						if(_map->_fields[p._x][p._y].getValue()!=0)
						{
							_Estart = p;
							if(_enemy1==0)
							{
								_enemy1 = new EnemyObject(_gc,_map->_tilesMap["enemy"],_map->_tilesMap["path_enemy1"],_Eziel);
								_enemy1->setWaypoints(_Estart,_Eziel);
								_enemy1->_agression=ENEMY_AGGRESSION;
								createPath(_enemy1);
							}
							else
							{
								if(_enemy2==0)
								{
									_enemy2 = new EnemyObject(_gc,_map->_tilesMap["enemy"],_map->_tilesMap["path_enemy2"],_Eziel);
									_enemy2->setWaypoints(_Estart,_Eziel);
									_enemy2->_agression=ENEMY_AGGRESSION;
									createPath(_enemy2);
								}
							}
							_Estart._x=-1;
							_Eziel._x=-1;
						}
					}
				}
				clan::Console::write_line("Set Enemy x:%1 y:%2",in_key.mouse_pos.x,in_key.mouse_pos.y);*/
				break;
			case mouse_middle:
				break;
			case mouse_wheel_up:
				break;
			case mouse_wheel_down:
				break;
				//nothing
		}
	} // if - mouse click occured inside app window
}

void App::createPath(EnemyObject* obj)
{
	_pathfinder->init(obj->_start, obj->_ziel, true);
	int length = _pathfinder->searchPath();
	if (length < 0)
	{
		// there is no path
		// clan::Console::write_line("Gegner: Keinen Weg gefunden.");
		_Eziel._x = -1;
		_Estart._x = -1;
		if (_enemy2 != 0)
		{
			delete _enemy2;
			_enemy2 = 0;
		}
		else
		{
			delete _enemy1;
			_enemy1 = 0;
		}
	}
	else
	{
		Position2D *path = new Position2D[length];
		_pathfinder->getPath(path);
		obj->setPath(path, length);
	}
}

void App::insertStart(Position2D pos)
{
	Position2D p = getHex(pos);
	if (_map->_fields[p._x][p._y].getValue() > 0)
		_start = new GameObject(canvas, _map->_tilesMap["start"], _map->_tilesMap["path"], p);
}

void App::insertZiel(Position2D pos)
{
	Position2D p = getHex(pos);
	if (_map->_fields[p._x][p._y].getValue() > 0)
		_ziel = new GameObject(canvas, _map->_tilesMap["target"], _map->_tilesMap["path"], p);
}

void App::deletePoints()
{
	if (_start != 0)
	{
		_start->deletePath();
		delete _start;
		_start = 0;
	}
	if (_ziel != 0)
	{
		delete _ziel;
		_ziel = 0;
	}
	/*if(_enemy1!=0)
	{
		delete _enemy1;
		_enemy1=0;
	}
	if(_enemy2!=0)
	{
		delete _enemy2;
		_enemy2=0;
	}*/
}

// convert pixel coordinates into hexagonal map coordinate system
Position2D App::getHex(Position2D pt) {
	// precalculate the 'hotspot' regions
	float scale_factor;
#if SMALL==0
	scale_factor = TILES_SCALE_FACTOR;
#else
	scale_factor = TILES_SCALE_FACTOR_SMALL;
#endif
	const float MAPTILE_HEIGHT_HOT = TILES_SIZE_Y * scale_factor * .77F;
	const float MAPTILE_UPPERTHIRD = MAPTILE_HEIGHT_HOT * .23F;
	const float ANGLE = 0.589F;		// 25°
	const float ANGLE2 = 1.8095F;	// 155°
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

// runs all x seconds
void App::updatePath()
{
	_map->clearPotentials();			// clear potential map
	// add enemies to potential map
	if (_enemy1 != 0)
		_map->setPotentials(_enemy1->getPosition(), _enemy1->_agression, POTENTIAL_SIZE);
	if (_enemy2 != 0)
		_map->setPotentials(_enemy2->getPosition(), _enemy2->_agression, POTENTIAL_SIZE);

	// recalculate path
	_pathfinder->init(_ziel->getPosition(), _start->getPosition());
	// remove path
	//_start->deletePath();
	int length = _pathfinder->searchPath();
	if (length < 0)
	{
		// there is no path
		clan::Console::write_line("Spieler: Keinen Weg gefunden.");
	}
	else
	{
		Position2D *path = new Position2D[length];
		_pathfinder->getPath(path);
		_start->setNewPath(path, length);
	}
}