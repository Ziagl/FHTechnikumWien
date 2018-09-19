#include "lrule.h"

LRule::LRule(char in, std::string pattern, int propability)
{
	_in = in;
	_propability = propability;

	for (unsigned int i = 0; i < pattern.length(); ++i)
	{
		_out.push_back(pattern[i]);
	}
}