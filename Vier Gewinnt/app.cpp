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
	desc.set_title("4 gewinnt");
	desc.set_size(clan::Sizef(WINDOW_WIDTH, WINDOW_HEIGHT), true);
	desc.set_allow_resize(true);

	window = clan::DisplayWindow(desc);
	canvas = clan::Canvas(window);
	font = clan::Font("Courir", 34);

	// connect the window close event
	sc.connect(window.sig_window_close(), [&]() {_quit = true; });

	// connect a keyboard handler to on_key_up()
	sc.connect(window.get_keyboard().sig_key_up(), clan::bind_member(this, &App::on_keyboard));

	// connect a mouse handler (only for example it is not used)
	sc.connect(window.get_mouse().sig_key_down(), this, &App::on_mouse);
	sc.connect(window.get_mouse().sig_key_dblclk(), this, &App::on_mouse);

	_quit = false;
	srand(static_cast<int>(time(0)));	// initialize random number generator

	init();								//initialize
}

bool App::update()
{
	// clear display with white background
	canvas.clear(clan::Colorf(0.0f, 0.0f, 0.0f));

	render();

	window.flip(1);

	return !_quit;
}

// keyboard key was pressed
void App::on_keyboard(const clan::InputEvent &key)
{
	switch (key.id) {
		case clan::keycode_escape:
		case clan::keycode_q:
			_quit = true; break;
		// move cursor with left and right arrow
		case clan::keycode_left:
			_cursor->moveLeft();
			break;
		case clan::keycode_right:
			_cursor->moveRight();
			break;
		// enter or space for add stone
		case clan::keycode_enter:
		case clan::keycode_space:
			compute();
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

void App::init()
{
	_cursor = new CursorPos(canvas);
	_map = new Map(canvas);
	_player = true;			// player 1
	_victory = false;
	text = "";
	
	_time = 0;
} // init()

void App::render()
{
	unsigned int time_loopstart = clan::System::get_time();

	_map->render();
	_cursor->render(_player);

	if (_victory)
	{
		clan::Sizef text_size = font.measure_text(canvas, text).bbox_size;
		font.draw_text(canvas, (WINDOW_WIDTH / 2) - (text_size.width / 2), WINDOW_HEIGHT / 2, text, clan::Colorf::green);
	}

	// time based rendering optimization
	window.flip(0);

	unsigned int time_loopend = clan::System::get_time();
	if (time_loopend - time_loopstart < 40)	//40 ms = 25 fps
	{
		clan::System::sleep(40 - (time_loopend - time_loopstart));
	}
} // render()

void App::compute()
{
	if (!_victory)
	{
		// add new player stone ane change current player
		if (_map->_field.setField(_cursor->getX(), _player))
		{
			_player = !_player;
			if (_map->getWinner() < 2 && _map->getWinner() >= 0)
				_victory = true;
			// render
			render();
			if (!_victory)
			{
				// run ai computation
				runAI();
				_player = !_player;
			}
			getWinner();
		}
	}
	if (_map->_field.getFree() == 0)
	{
		_victory = true;
		text = "Draw";
	}
} // compute()

void App::runAI()
{
	GameTree gametree(_map->_field.getcopy(), _player, TREE_DEEP);
	gametree.evaluateLeafes();
	gametree.minimax();
	int turn = gametree.getTurn();
	if (turn != -1)
		_map->_field.setField(turn, _player);
}

void App::getWinner()
{
	// check for a winner and set victory if needed
	switch (_map->getWinner())
	{
		case 0: 
			text = "Red wins!"; 
			_victory = true; 
			break;
		case 1: 
			text = "Yellow wins!";
			_victory = true; 
			break;
	}
}