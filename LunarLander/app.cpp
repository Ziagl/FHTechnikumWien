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
	desc.set_title("Lunar Lander");
	desc.set_size(clan::Sizef(WINDOW_WIDTH, WINDOW_HEIGHT), true);
	desc.set_allow_resize(true);

	window = clan::DisplayWindow(desc);
	canvas = clan::Canvas(window);
	font = clan::Font("Courir", 35);
	font1 = clan::Font("Courir", 30);

	// connect the window close event
	sc.connect(window.sig_window_close(), [&]() {quit = true; });

	// connect a keyboard handler to on_key_up()
	sc.connect(window.get_keyboard().sig_key_up(), clan::bind_member(this, &App::on_keyboard_up));
	sc.connect(window.get_keyboard().sig_key_down(), clan::bind_member(this, &App::on_keyboard_down));

	quit = false;
	_time = 0;
	srand(static_cast<int>(time(0)));	// initialize random number generator

	// init
	lander = Lander(canvas);
	Phys_gravity = Gravity();
	Phys_engine = Gravity();
	Phys_gravity._gravity = GRAVITY_MOON * DT;	 // 1,62 m/s * dt
	Phys_engine._gravity = -SPEED_ENGINE * DT;   // 2 m/s * dt
	earth = BgObject(canvas, ".\\Data\\Tiles\\earth.png", 200.0f, 100.0f);
	moon = BgObject(canvas, ".\\Data\\Tiles\\lunar_surface.png", 0.0f, 500.0f);
	stars = BgObject(canvas, ".\\Data\\Tiles\\stars.png", 0.0f, 0.0f);

	// register physical objects
	Phys_gravity.registerClass(&lander);

} // constructor

bool App::update()
{
	unsigned int time_loopstart = 0;
	unsigned int time_loopend = 0;

	time_loopstart = clan::System::get_time();
	canvas.clear(clan::Colorf(0.0f, 0.0f, 0.0f));

	stars.render(1.1);
	earth.render();
	moon.render(0.8);

	// as long as eagle has not landed
	if (!lander._landed)
	{
		Phys_gravity.compute();
		Phys_engine.compute();
		lander.move(_time);
	}
	lander.render();

	//Fonts
	std::stringstream str;
	str << static_cast<int>(lander._speed._y*INTERFACE_SPEED);
	clan::Colorf color = clan::Colorf::green;
	if (lander._speed._y*INTERFACE_SPEED > MAX_IMPACT_SPEED)
		color = clan::Colorf::red;
	font.draw_text(canvas, 200.0f, 480.0f, str.str(), color);
	font1.draw_text(canvas, 240.0f, 480.0f, "m/s", color);

	std::stringstream str1;
	str1 << static_cast<int>(lander._fuel);
	font.draw_text(canvas, 200.0f, 440.0f, str1.str(), clan::Colorf::yellow);
	font1.draw_text(canvas, 240.0f, 440.0f, "l", clan::Colorf::yellow);

	std::stringstream str2;
	str2 << static_cast<int>(lander._t);
	font.draw_text(canvas, 200.0f, 400.0f, str2.str(), clan::Colorf::white);
	font1.draw_text(canvas, 240.0f, 400.0f, "s", clan::Colorf::white);

	if (lander._landed)
	{
		if (lander._save)
			font.draw_text(canvas, 60.0f, 250.0f, "Touchdown!", clan::Colorf::green);
		else
			font.draw_text(canvas, 72.0f, 250.0f, "Crashed!", clan::Colorf::red);
	}

	window.flip();

	// timer
	time_loopend = clan::System::get_time();
	_time = time_loopend - time_loopstart;

	return !quit;
}

void App::on_keyboard_down(const clan::InputEvent &key)
{
	switch (key.id) {
		case clan::keycode_space:
			lander._engine = true;
			if (lander._fuel > 0.0 && lander.registered == false)
			{
				lander.registered = true;
				Phys_engine.registerClass(&lander);
			}
			break;
			break;
	}

} // on_keyDown

void App::on_keyboard_up(const clan::InputEvent &key)
{
	switch (key.id) {
	case clan::keycode_q:
		quit = true;
		break;
	case clan::keycode_space:
		lander._engine = false;
		Phys_engine.deregisterClass(&lander);
		lander.registered = false;
		break;
	}

} // on_keyUp

