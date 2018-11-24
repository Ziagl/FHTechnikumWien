#pragma once

#ifdef WIN32
#pragma warning(disable : 4786)
#endif

#include "precomp.h"

#define DEBUG 0
#define SEED 97367
#define LEVEL 1			//1-4

#define WINDOW_WIDTH  1080
#define WINDOW_HEIGHT 705

// AntColony
#define PHEROMONES_MIN 0.1f
#define PHEROMONES_LOWER 0.03f
#define PATH_MAX 15*12*4

// path finding weights
#define WEIGHT_PHEROMONES 1.0f
#define WEIGHT_DIRECTION 0.6f

// board dimension
#define LEVEL_WIDTH 15
#define LEVEL_HEIGHT 12

// timing
#define MOVEMENT_PLAYER_UPDATE 1000		// interpolation steps between two points

// HEXAGON
#define NEIGHBOURS 6

// map
#define TILES_SCALE_FACTOR 1
#define TILES_SCALE_FACTOR_SMALL 0.5
#define TILES_SIZE_X 70//140
#define TILES_SIZE_Y 76//160
#define TILES_OFFSET_X 35//70
#define TILES_OFFSET_Y 57//120

// globals
static const int xofs[NEIGHBOURS] = { -1, 0, 0, -1, -1, 1 };
static const int xofsEven[NEIGHBOURS] = { 0, 1, 1, -1, 0, 1 };
static const int yofs[NEIGHBOURS] = { -1, -1, 1, 0, 1, 0 };