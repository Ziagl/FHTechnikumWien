#include <iostream>

int main(void)
{
	double dt = 0.01;
	double dv;
	double v = 0.0, t = 0.0;

	double a = 10.0;
	double s = 30.0;

	dv = a * dt;

	while (s > 0)
	{
		v += dv;
		s -= dt * v;
		t += dt;
		std::cout << "s=" << s << " v=" << v << " t=" << t << std::endl;
	}

	return 0;
}