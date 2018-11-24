#pragma once

#include "globals.h"
#include "map.h"
#include "ant.h"

class App : public clan::Application
{
public:
	App();
	bool update() override;

public:
	void on_quit(void) { _quit = true; }

	//-- private member variables
private:
	bool _quit;			// quit program
	bool _renderall;

	// game objects
	Map* _map;
	Ant* _start;
	Position2D* _anthill;
	int _antCounter;
	GameObject* _ziel;

	// implementaion
private:
	// input listener
	void on_keyboard(const clan::InputEvent &key);
	void on_mouse(const clan::InputEvent &key);

	// functions
	void init(void);
	void render(void);
	void compute(void);

	void insertStart(Position2D pos);
	void insertZiel(Position2D pos);
	Position2D getHex(Position2D pt);

	clan::SlotContainer sc;
	clan::DisplayWindow window;
	clan::Canvas canvas;
}; // class Game
