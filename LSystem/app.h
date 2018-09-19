#pragma once

#include "precomp.h"
#include "lrule.h"
#include "lsystem.h"

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
	void init(int tree);
	void render(void);
	void compute(void);

	bool _quit;			// stop program?

	LSystem lsystem;	// used L-System
	int length;			// hight of plant elements
	std::string rule;	// string to render

	std::list<char> _string;		// current string
	std::vector<LRule> _rules;		// rules
	int _angel;

	clan::SlotContainer sc;
	clan::DisplayWindow window;
	clan::Canvas canvas;
};