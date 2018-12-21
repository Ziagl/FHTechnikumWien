#pragma once

#include "globals.h"
#include "fireworks_particle.h"
#include "phy_gravity.h"
#include "phy_impulse.h"
#include "phy_explosion.h"
#include "phy_friction.h"

class App : public clan::Application
{
public:
	App();
	~App() {};
	bool update() override;

public:
	void on_quit(void) { _quit = true; }

private:
	bool _quit;			// quit program?

	unsigned int _time;

	Gravity* _gravitation;
	Impulse* _startimpuls;
	Explosion* _explosion;
	Friction* _friction;
	std::vector<FireworksParticle*> _particles;
	long _particlecounter;
	long _maxParticle;

	clan::SlotContainer sc;
	clan::DisplayWindow window;
	clan::Canvas canvas;

	// implementation
private:
	// input listener
	void on_keyboard(const clan::InputEvent &key);
	void on_mouse(const clan::InputEvent &key);

	// functions
	void init(void);
	void render(void);
	void compute(void);

	int irand(int a, int b);
	FireworksParticle* getFireworksParticle(float x, float y, float lifetime, float size, int tiefe, int timer);
}; // class Game