#pragma once

#include "precomp.h"

// a rule defines a pattern for a given input
class LRule
{
public:
	LRule(char in, std::string pattern, int propability);

	char _in;					// input
	std::list<char> _out;		// possible pattern
	int _propability;			// propability for that pattern
};