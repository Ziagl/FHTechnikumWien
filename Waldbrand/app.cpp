#include "precomp.h"
#include "globals.h"
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

	// Set the window
	clan::DisplayWindowDescription desc;
	desc.set_title("Waldbrand!");
	desc.set_size(clan::Sizef(WINDOW_HEIGHT+7, WINDOW_WIDTH+27), false);
	desc.set_allow_resize(false);

	window = clan::DisplayWindow(desc);
	canvas = clan::Canvas(window);

	// connect the window close event
	sc.connect(window.sig_window_close(), [&]() {_quit = true; });

	// connect a keyboard handler to on_key_up()
	sc.connect(window.get_keyboard().sig_key_up(), clan::bind_member(this, &App::on_keyboard));

	// connect a mouse handler (only for example it is not used)
	sc.connect(window.get_mouse().sig_key_down(), this, &App::on_mouse);
	sc.connect(window.get_mouse().sig_key_dblclk(), this, &App::on_mouse);

	init();
}

bool App::update()
{
	if (!_pause)
		compute();

	// clear display in a dark blue nuance
	canvas.clear(clan::Colorf(1.0f, 1.0f, 1.0f));

	render();

	window.flip(1);

	return !_quit;
}

// keyboard key was pressed
void App::on_keyboard(const clan::InputEvent &key)
{
	switch (key.id) {
	case clan::keycode_escape:
		_quit = true;
		break;
	case clan::keycode_space:
		_pause = !_pause;
		break;
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

void App::init(void) {
	_quit = false;
	_pause = false;
	_currentStep = 0;
	_board = 0;
	_nextboard = 1;
	_empty = _fire = _woods = 0;
	srand(static_cast<int>(time(0)));			// initialize random number generator

	for (int i = 0; i<WOOD_HEIGHT; ++i)
		for (int j = 0; j<WOOD_WIDTH; ++j)
			_wood[_board][i][j] = getCell();
	clan::Console::write_line("Leer: %1 Feuer: %2 Wald: %3", _empty, _fire, _woods);
}

void App::render(void) {
	for (int i = 0; i<WOOD_HEIGHT; ++i)
		for (int j = 0; j<WOOD_WIDTH; ++j)
		{
			clan::Colorf color;
			switch (_wood[_board][i][j])
			{
			case EMPTY: color = clan::Colorf::black; break;
			case WOOD: color = clan::Colorf::darkgreen; break;
			case FIRE: color = clan::Colorf::red; break;
			}
			canvas.fill_rect(clan::Rectf((float)i, (float)j, (float)i + 1, (float)j + 1), color);
		}
}

void App::compute(void) {
	for (int i = 0; i<WOOD_HEIGHT; ++i)
		for (int j = 0; j<WOOD_WIDTH; ++j)
		{
			switch (_wood[_board][i][j])
			{
			case EMPTY:
				// if it is an empty cell --> new tree or stay empty
				if (irand(MIN, MAX)<PROPABILITY_NEW_WOOD)
					_wood[_nextboard][i][j] = WOOD;
				else
					_wood[_nextboard][i][j] = EMPTY;
				break;
			case WOOD:
				// if neighbor is on fire --> tree also starts burning
				if (hasBurningNeighbour(_board, i, j))
				{
					_wood[_nextboard][i][j] = FIRE;
				}
				// if there is no neighbor --> should it start burning? (flying sparks)
				else
				{
					if (irand(MIN, MAX)<PROPABILITY_NEW_FIRE)
						_wood[_nextboard][i][j] = FIRE;
					else
						_wood[_nextboard][i][j] = WOOD;
				}
				break;
			case FIRE:
				// fire turns to empty field
				_wood[_nextboard][i][j] = EMPTY;
				break;
			}
		}

	// change boards
	_board = _nextboard;
	if (_nextboard == 1)
		_nextboard = 0;
	else
		_nextboard = 1;

	// count current generation
	_currentStep++;
}

unsigned char App::getCell() {
	int wert = irand(MIN, MAX);

	if (wert<PROPABILITY_WOOD)
	{
		_woods++;
		return WOOD;
	}
	else
	{
		if (wert - PROPABILITY_WOOD<PROPABILITY_FIRE)
		{
			_fire++;
			return FIRE;
		}
		else
		{
			_empty++;
			return EMPTY;
		}
	}
}

inline bool App::hasBurningNeighbour(int board, int i, int j)
{
	// computation without wrap around!
	return get(board, i - 1, j - 1) || get(board, i - 1, j) || get(board, i - 1, j + 1) ||
		get(board, i, j - 1) || get(board, i, j) || get(board, i, j + 1) ||
		get(board, i + 1, j - 1) || get(board, i + 1, j) || get(board, i + 1, j + 1);
}

inline bool App::get(int board, int i, int j)
{
	if (i<0 || i >= WOOD_HEIGHT || j<0 || j >= WOOD_WIDTH)
		return false;
	else
		return _wood[board][i][j] == FIRE;
}

int App::irand(int a, int b) {
	double r = b - a + 1;
	return a + static_cast<int>(r * rand() / (RAND_MAX + 1.0));
}