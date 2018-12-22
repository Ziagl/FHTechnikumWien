#pragma once

#include "precomp.h"
#include "globals.h"
#include "lander.h"
#include "gravity.h"
#include "bgobject.h"

class App : public clan::Application
{
public:
	App();
	~App() {};
	bool update() override;						// game loop

	void on_quit() { quit = true; }				// quit the game

	Lander lander;								// lander object
	Gravity Phys_gravity;
	Gravity Phys_engine;
	BgObject earth;
	BgObject moon;
	BgObject stars;

	bool quit;
private:
	// input listener
	void on_keyboard_down(const clan::InputEvent &key);
	void on_keyboard_up(const clan::InputEvent &key);

	clan::SlotContainer sc;
	clan::DisplayWindow window;
	clan::Canvas canvas;
	clan::Font font;
	clan::Font font1;
	unsigned long _time;
};
