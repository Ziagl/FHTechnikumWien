#pragma once

#include "lrule.h"

class LSystem
{
public:
	LSystem() {};

	void init(std::string startString, int angel);	// create initial state
	void computeRules();				//ersetzt die Chars mit den Pattern (verfeinert die Pflanze um einen Schritt)
	void clearRules();
	void addRule(char in, std::string pattern, int propability);	// add a new role
	std::string get();					// get L-System as string

	int irand(int a, int b);

	std::list<char> _string;		// current string
	std::vector<LRule> _rules;		// current replacement rule
	int _angel;
};