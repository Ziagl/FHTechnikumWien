#include "lsystem.h"

// create initial state
void LSystem::init(std::string startString, int angel)
{
	_string.clear();

	for (unsigned int i = 0; i < startString.length(); ++i)
		_string.push_back(startString[i]);

	_angel = angel;
}

// clear all rules
void LSystem::clearRules()
{
	_rules.clear();
}


void LSystem::computeRules()
{
	// one step
	std::list<char> l;

	// loop all chars of string
	for (std::list<char>::iterator it = _string.begin();
		it != _string.end();
		++it)
	{
		bool ok = true;

		// new random number
		int prop = irand(1, 100);

		// test each char if rule applies
		for (std::vector<LRule>::iterator itv = _rules.begin();
			itv != _rules.end();
			++itv)
		{
			// add new rule
			if (*it == itv->_in)
			{
				// test propability 
				if (prop <= itv->_propability)
				{
					for (std::list<char>::iterator itout = itv->_out.begin();
						itout != itv->_out.end();
						++itout)
						l.push_back(*itout);
					ok = false;
					break;
				}
				// if not process to next rule
				else
				{
					prop = prop - itv->_propability;
				}
			}
		}

		if (ok)
			l.push_back(*it);
	}

	_string.clear();
	_string = l;
}

// add a new role
void LSystem::addRule(char in, std::string pattern, int propability)
{
	_rules.push_back(LRule(in, pattern, propability));
}

// get L-System as string
std::string LSystem::get()
{
	std::stringstream str;

	for (std::list<char>::iterator it = _string.begin();
		it != _string.end();
		++it)
	{
		str << *it;
	}

	return str.str();
}


int LSystem::irand(int a, int b) {
	double r = b - a + 1;
	return a + static_cast<int>(r * rand() / (RAND_MAX + 1.0));
}