#pragma once

#ifdef WIN32
#pragma warning(disable : 4786)
#endif

// ClanLib includes
#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#ifdef WIN32
#include <ClanLib/d3d.h>
#endif
#include <ClanLib/gl.h>

// boost includes
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/variate_generator.hpp>

#include <math.h>
#include <ctime>
#include <vector>

#define WINDOW_HEIGHT 600
#define WINDOW_WIDTH  800