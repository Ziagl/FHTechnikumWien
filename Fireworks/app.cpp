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
	desc.set_title("Fireworks");
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

} // constructor

bool App::update()
{
	unsigned int time_loopstart = 0;
	unsigned int time_loopend = 0;

	time_loopstart = clan::System::get_time();
	canvas.clear(clan::Colorf(0.0f, 0.0f, 0.0f));

	compute();

	render();

	window.flip();

	// timer
	time_loopend = clan::System::get_time();
	_time = time_loopend - time_loopstart;

	return !_quit;
}

void App::on_keyboard(const clan::InputEvent &key)
{
	switch (key.id) {
		case clan::keycode_q:
			_quit = true; break;
	}

} // on_keyDown

void App::on_mouse(const clan::InputEvent &key) 
{
	// mouse position
	const float x = key.mouse_pos.x;
	const float y = key.mouse_pos.y;

	// check if the mouse click occured inside the application window
	if (x >= 0 && x <= WINDOW_WIDTH &&
		y >= 0 && y <= WINDOW_HEIGHT)
	{
		//einen neuen Partikel erstellen
		FireworksParticle *f = getFireworksParticle(static_cast<float>(x), static_cast<float>(y), MAX_LIFETIME_MAX, PARTICLE_SIZE, 0, TIMER);
		_gravitation->registerClass(f);
		_startimpuls->registerClass(f);
	} // if - mouse click occured inside app window

} // on_mousePress()

void App::init(void)
{
	// initially create enough particles for system
	_maxParticle = 0;
	for (int i = 0; i <= MAX_TIEFE; ++i)
		_maxParticle += static_cast<long>(pow(static_cast<double>(PARTICLE_NUMBER), i));
	_maxParticle *= 2;

	clan::Console::write_line("Max Particles: %1", _maxParticle);

	_particlecounter = 0;

	for (long i = 0; i < _maxParticle; ++i)
		_particles.push_back(new FireworksParticle(0, 0, 0));

	clan::Vec2f _gravity;
	_gravity.x = 0.0f;
	_gravity.y = 1.0f * static_cast<float>(GRAVITY_EARTH * DT);
	_gravitation = new Gravity(_gravity);

	clan::Vec2f grav;
	grav.x = 0.0f;
	grav.y = -1.0f * START_IMPULSE;
	_startimpuls = new Impulse(grav);

	_explosion = new Explosion(EXPLOSION_MIN, EXPLOSION_MAX);
} // init

void App::render(void)
{
	// draw each particle
	for (std::vector<FireworksParticle*>::iterator it = _particles.begin();
		it != _particles.end();
		++it)
	{
		// if particle lifetime is over
		if ((*it)->_lifetime <= 0.0f && (*it)->_active == true)
		{
			(*it)->_lifetime = 0.0f;
			(*it)->_active = false;
			_gravitation->deregisterClass(*it);
		}
		// if particle lives
		if ((*it)->_active)
		{
			// use force
			(*it)->update(_time);

			// reset force
			(*it)->resetForce();

			float x = (*it)->_pos._x;
			float y = (*it)->_pos._y;
			float size = (*it)->_size;
			int tiefe = (*it)->_tiefe;

			// particle should explode
			if ((*it)->_explosionstimer < 0.0f && (*it)->_tiefe < MAX_TIEFE)
			{
				// particle is dead now
				(*it)->_lifetime = 0.0f;
				(*it)->_active = false;
				_gravitation->deregisterClass(*it);

				tiefe++;
				for (int i = 0; i < PARTICLE_NUMBER; ++i)
				{
					FireworksParticle *f = getFireworksParticle(x, y, static_cast<float>(irand(MAX_LIFETIME_MIN, MAX_LIFETIME_MAX)), size / 2, tiefe, (*it)->_maxtimer);
					f->_speed._x = (*it)->_speed._x;
					f->_speed._y = (*it)->_speed._y;
					f->_r = irand(0, 255) / 255.0f;
					f->_b = irand(0, 255) / 255.0f;
					f->_g = irand(0, 255) / 255.0f;

					//Physik
					_gravitation->registerClass(f);
					_explosion->registerClass(f);
				}
			}

			clan::Colorf color;
			color.set_blue((*it)->_b);			// blue part
			color.set_green((*it)->_g);			// green part
			color.set_red((*it)->_r);			// red part
			color.set_alpha((*it)->_lifetime / (*it)->_maxlifetime);
			canvas.fill_rect(clan::Rectf(x - size, y - size, x + size, y + size), color);
		}
	}
} // render

void App::compute()
{
	// use physics
	_gravitation->compute();
	_startimpuls->compute();
	_explosion->compute();
} // compute

int App::irand(int a, int b) {
	double r = b - a + 1;
	return a + static_cast<int>(r * rand() / (RAND_MAX + 1.0));
}

FireworksParticle* App::getFireworksParticle(float x, float y, float lifetime, float size, int tiefe, int timer)
{
	FireworksParticle *f = 0;

	// get a free not used particle
	if (_particlecounter == _maxParticle)
		_particlecounter = 0;
	f = _particles[_particlecounter];
	_particlecounter++;

	f->_mass = PARTICLEMASS;
	f->_pos._x = x;
	f->_pos._y = y;
	f->_speed._x = 0.0f;
	f->_speed._y = 0.0f;
	f->_r = 1.0f;
	f->_g = 1.0f;
	f->_b = 0.0f;
	f->_maxlifetime = lifetime;
	f->_lifetime = lifetime;
	f->_tiefe = tiefe;
	f->_size = size;
	f->_active = true;
	f->_maxtimer = f->_explosionstimer = irand(timer / 2, timer + timer / 2);
	f->resetForce();
	return f;
}