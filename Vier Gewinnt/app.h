#pragma once

#include "precomp.h"
#include "globals.h"
#include "map.h"
#include "gametree.h"
#include "cursor.h"

class App : public clan::Application
{
public:
	App();
	bool update() override;

private:
	// input listener
	void on_keyboard(const clan::InputEvent &key);
	void on_mouse(const clan::InputEvent &key);

	// game methods
	void init(void);
	void render(void);
	void compute(void);

	// helper methods
	void runAI();
	void getWinner();
	void on_quit() { quit = true; }

	bool _quit;				// stop program?
	Map *_map;
	CursorPos *_cursor;
	bool _player;
	bool quit;
	bool _victory;
private:
	unsigned long _time;	// for timebased rendering --> 25fps
	std::string text;

	clan::SlotContainer sc;
	clan::DisplayWindow window;
	clan::Canvas canvas;
	clan::Font font;
};
