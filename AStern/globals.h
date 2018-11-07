#pragma once

#include "precomp.h"

// debugmode on (1) or off (0) 
#define DEBUG 1

// (1) map will be displayed half the size
#define SMALL 0

// board Dimension
#define LEVEL_WIDTH 15
#define LEVEL_HEIGHT 12

// window size (for large and small option)
#define WINDOW_SIZE_X 1100
#define WINDOW_SIZE_SMALL_X 560
#define WINDOW_SIZE_Y 740
#define WINDOW_SIZE_SMALL_Y 390

// map params
#define TILES_SCALE_FACTOR 1
#define TILES_SCALE_FACTOR_SMALL 0.5
#define TILES_SIZE_X 70//140
#define TILES_SIZE_Y 76//160
#define TILES_OFFSET_X 35//70
#define TILES_OFFSET_Y 57//120

// timing
#define MOVEMENT_PLAYER_UPDATE 500		// interpolations between two points
#define PATH_UPDATE_TIME 3000			// regenerate path all x milliseconds

// HEXAGON
#define NEIGHBOURS 6

// AI
#define POTENTIAL_SIZE 3				// radius around enemy
#define ENEMY_AGGRESSION 9				// aggression of enemy

// global arrays
static const int xofs[NEIGHBOURS] = { -1, 0, -1, 1, -1, 0 };		// neighbours X
static const int xofsEven[NEIGHBOURS] = { 0, 1, -1, 1, 0, 1 };
static const int yofs[NEIGHBOURS] = { -1, -1, 0, 0, 1, 1 };			// neighbours Y
