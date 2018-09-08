#pragma once

#include "globals.h"

class App : public clan::Application
{
public:
	App();
	bool update() override;
private:
	// input listener
	void on_keyboard(const clan::InputEvent &key);
	void on_mouse(const clan::InputEvent &key);

	bool _quit;			// program quit
	bool _pause;		// program pause
	int _currentStep;	// generation

	int _empty;
	int _fire;
	int _woods;

	unsigned char _wood[2][WOOD_WIDTH][WOOD_HEIGHT];
	int _board;
	int _nextboard;

	// game methods
	void init(void);
	void render(void);
	void compute(void);

	// helper methods
	inline bool hasBurningNeighbour(int board, int i, int j);
	inline bool get(int board, int i, int j);
	unsigned char getCell();	// returns one newly created cell
	int irand(int a, int b);

	clan::SlotContainer sc;
	clan::DisplayWindow window;
	clan::Canvas canvas;
};