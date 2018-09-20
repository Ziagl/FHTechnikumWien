#pragma once

#include "precomp.h"
#include "globals.h"
#include "genom.h"

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
	void createPopulation(int size);					// create population				
	void evaluatePopulation(int &sum);					// evaluate population
	void evaluateIndividual(Genom* o);					// evaluate individual (fitness function)
	int irand(int a, int b);
	bool checkDamen(int x1, int y1, int x2, int y2);	// returns true if tqwo queens won't beat
	void evolutePopulation(int f, bool mutation);		// one evolution step
	Genom* GetParent(int k);							// returns a parent from population
	void copulateParents(Genom* p1, Genom* p2, bool mutation);
	void selectionOfPopulation();						// individuals with bad fitness now die

	bool _quit;			// stop program?
	int generation;
	std::list<Genom*>::iterator it_g;

	// list of individuals --> better performance as vector, 
	// because elements are often moved
	std::list<Genom*> population;
	std::list<Genom*> temp_population;

	clan::SlotContainer sc;
	clan::DisplayWindow window;
	clan::Canvas canvas;
	clan::Font font;
	clan::Image queen;
};