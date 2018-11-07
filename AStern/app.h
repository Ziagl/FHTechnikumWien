#pragma once

#include "globals.h"
#include "position2d.h"
#include "enemyobject.h"
#include "map.h"
#include "astar.h"

class App : public clan::Application
{
public:
	App();
	~App();
	bool update() override;

	void on_quit() { _quit = true; }

	// variables
	Map *_map;							// map of tiles
	bool _player;
	bool _quit;
	bool _victory;
private:
	void on_keyboard(const clan::InputEvent &key);
	void on_mouse(const clan::InputEvent &key);

	// functions
	void init(void);
	void render(void);
	void compute(void);

	Position2D getHex(Position2D pt);
	void createPath(EnemyObject* obj);
	void insertStart(Position2D pos);
	void insertZiel(Position2D pos);
	void deletePoints();
	void updatePath();

	// variables
	unsigned long _time;						// for time based rendering --> 25fps

	// AStar
	AStar *_pathfinder;

	// game objects
	GameObject* _start;
	GameObject* _ziel;

	// enemies are special game objects
	EnemyObject* _enemy1;
	EnemyObject* _enemy2;

	Position2D _Estart, _Eziel;

	clan::SlotContainer sc;
	clan::DisplayWindow window;
	clan::Canvas canvas;

	unsigned int time_loopstart;
	unsigned int time_loopend;
	unsigned int time_elapsed;
	unsigned int time_pathupdater;
};
