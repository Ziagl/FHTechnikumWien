#pragma once

#include "globals.h"
#include "sheep.h"
#include "map.h"
#include "log.h"

class App : public clan::Application
{
public:
	App();
	~App();
	bool update() override;

public:
	void on_quit(void) { _quit = true; }

private:
	bool _quit;			// quit program
	bool _pause;
	int _round;			// current step of simulation
	int _grascounter;
	unsigned int _timer;			// timer

	// game objects
	Map* _map;						// map
	std::vector<Sheep> _sheeps;		// sheep

	// implementation
private:
	// input listener
	void on_keyboard(const clan::InputEvent &key);
	void on_mouse(const clan::InputEvent &key);

	// functions
	void init(void);
	void render(void);
	void compute(void);

	void createSheep(Position2D &pos);		// creates a new sheep on map
	Position2D getHex(Position2D &pt);

	clan::SlotContainer sc;
	clan::DisplayWindow window;
	clan::Canvas canvas;
}; // class Game
