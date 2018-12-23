#pragma once

#include "precomp.h"
#include "position2d.h"
#include "timer.h"

class App : public clan::Application
{
public:
	//-- construction / destruction
	App();
	bool update() override;

public:
	void on_quit(void) { m_bQuit = true; }

	//-- private members
private:
	clan::SlotContainer sc;
	clan::DisplayWindow window;
	clan::Canvas canvas;

	bool m_bQuit;

	typedef std::vector<Position2D> T_points;
	T_points m_vPoints;
	T_points m_vHull;
	int m_currentStep;

private:
	// input listener
	void on_keyboard(const clan::InputEvent &key);
	void on_mouse(const clan::InputEvent &key);

	void render(void);

	void calculateHull(int in_step = 0);
	void calculateHullStep(void);
	void createRandomPoints(int in_count);
}; // class App