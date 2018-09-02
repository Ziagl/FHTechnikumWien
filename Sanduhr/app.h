#pragma once

#include "pattern.h"

class App : public clan::Application
{
public:
	App();
	bool update() override;
private:
	// Input Listener
	void on_keyboard(const clan::InputEvent &key);
	void on_mouse(const clan::InputEvent &key);

	// Game Funktionen
	void init(void);
	void render(void);
	void compute(void);

	// HilfsFunktionen
	inline void compute_pattern(int x, int y, int board_in, int board_out);
	void init_pattern();
	int irand(int a, int b);	//Zufallswertfunktion

	bool quit = false;
	bool show_fps = false;
	bool _pause;
	int _currentStep;	//Generation

	unsigned char _sand[2][SAND_HEIGHT][SAND_WIDTH];
	int _board;
	int _nextboard;

	std::vector<Pattern> p_vec;

	float _time;
	float _lasttime;

	int _mode;

	clan::SlotContainer sc;
	clan::DisplayWindow window;
	clan::Canvas canvas;
	clan::Font font;
	clan::GameTime game_time;
};