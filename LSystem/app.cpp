#include "precomp.h"
#include "position2d.h"
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
	desc.set_title("Pflanze");
	desc.set_size(clan::Sizef(WINDOW_HEIGHT+7, WINDOW_WIDTH+27), true);
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

	init(0);		//initialize
}

bool App::update()
{
	// clear display with white background
	canvas.clear(clan::Colorf(1.0f, 1.0f, 1.0f));

	render();

	window.flip(1);

	return !_quit;
}

// keyboard key was pressed
void App::on_keyboard(const clan::InputEvent &key)
{
	switch (key.id) {
		case clan::keycode_q: _quit = true;break;
		case clan::keycode_space: compute(); break;
		case clan::keycode_1: init(0); break;
		case clan::keycode_2: init(1); break;
		case clan::keycode_3: init(2); break;
		case clan::keycode_4: init(3); break;
		case clan::keycode_5: init(4); break;
		case clan::keycode_6: init(5); break;
		case clan::keycode_7: init(6); break;
		case clan::keycode_8: init(7); break;
		case clan::keycode_down: length = length / 2; break;
		case clan::keycode_up: length = length * 2; break;
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

void App::init(int tree)
{
	lsystem.clearRules();

	switch (tree)
	{
		case 0: lsystem.init("F", 30);
			lsystem.addRule('F', "F[+F[+F]-F][-F]F", 100);
			break;
		case 1:	lsystem.init("F", 20);
			lsystem.addRule('F', "F[-F[-F]+F][+F]F", 50);
			lsystem.addRule('F', "F[+F[+F]-F][-F]F", 50);
			break;
		case 2: lsystem.init("F", 30);
			lsystem.addRule('F', "F[+F]F[-F]F", 33);
			lsystem.addRule('F', "F[-F]F[+F]F", 33);
			lsystem.addRule('F', "F[-FF-F]F", 34);
			break;
		case 3: lsystem.init("F", 30);
			lsystem.addRule('F', "F[+F]F[-F]F", 100);
			break;
		case 4: lsystem.init("F", 45);
			lsystem.addRule('F', "[+F][-F]FF", 100);
			break;
		case 5: lsystem.init("F", 338);
			lsystem.addRule('F', "FF+[+F-F-F]-[-F+F+F]", 100);
			break;
		case 6: lsystem.init("X", 30);
			lsystem.addRule('X', "F[+FX]-[-FX]FX", 100);
			lsystem.addRule('F', "FF", 100);
			break;
		case 7: lsystem.init("X", 30);
			lsystem.addRule('X', "F[+FX][-FX]FFX", 100);
			lsystem.addRule('F', "FF", 100);
			break;
	}
	rule = lsystem.get();
	length = OFFSET;
}// init()

void App::render(void) 
{
	Position2D start(WINDOW_WIDTH / 2, WINDOW_HEIGHT);
	Position2D end = start;
	clan::Colorf color = clan::Colorf::green;
	std::stack<Position2D> posSave;
	std::stack<clan::Vec2f> vecSave;
	clan::Vec2f v(0.0f, 0.0f + length);
	clan::Vec2f v0(0.0f, 0.0f);

	for (unsigned int i = 0; i < rule.length(); ++i)
	{
		switch (rule[i])
		{
			case 'F': end._x = start._x + (int)v.x;
				end._y = start._y - (int)v.y;
				canvas.draw_line((float)start._x, (float)start._y, (float)end._x, (float)end._y, color);
				break;
			case '+': v = v.rotate(v0, clan::Angle::from_degrees((float)lsystem._angel));
				break;
			case '-': v = v.rotate(v0, clan::Angle::from_degrees(-(float)lsystem._angel));
				break;
			case '[': posSave.push(end);
				vecSave.push(v);
				break;
			case ']': end = posSave.top();
				posSave.pop();
				v = vecSave.top();
				vecSave.pop();
				break;
		}
		start = end;
	}
} // render()

void App::compute()
{
	lsystem.computeRules();
	rule = lsystem.get();
} // compute()