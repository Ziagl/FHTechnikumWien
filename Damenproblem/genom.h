#pragma once

class Genom
{
public:
	int g[8];
	int fitness;

	Genom();
	Genom(int a, int b, int c, int d, int e, int f, int g, int h) {
		this->g[0] = a;
		this->g[1] = b;
		this->g[2] = c;
		this->g[3] = d;
		this->g[4] = e;
		this->g[5] = f;
		this->g[6] = g;
		this->g[7] = h;
		fitness = 0;
	};

	Genom* copy()
	{
		return new Genom(g[0], g[1], g[2], g[3], g[4], g[5], g[6], g[7]);
	};
};