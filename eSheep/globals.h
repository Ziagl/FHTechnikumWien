#pragma once

#include "precomp.h"

#define DEBUG 1			// 0..no debug info
						// 1..debug info for sheeps
						// 2..with console window
#define SEED 97367
#define LEVEL 0

// timing
#define TICKTIMER 1000			// milliseconds for one tick
#define GRASTIMER 5				// all x ticks gras grows

// gras growth
#define SPREADGRAS 8			// 0-10 >= SPREADGRAS-->neighbour gets new gras, default: 10
#define DIRTGRAS 2				// 0-10 >= DIRTGRAS-->turn dirt to gras, default: 0

#define WINDOW_WIDTH  1080
#define WINDOW_HEIGHT 705

// eSheep
// attribute
#define SHEEP_CHILDHOOD 50
#define SHEEP_ADULTHOOD 150
#define SHEEP_SENIOR 255

#define SHEEP_MIN 0
#define SHEEP_MAX 255

#define DESIRE_HUNGER 5
#define DESIRE_SOZIAL 5
#define DESIRE_HYGIENE 5
#define DESIRE_SEXUALITY 5
#define DESIRE_COMBAT 5
#define SURVIVAL_EMERGENCY 180	// hunger, social and hygiene have priority before sexuality and combat

#define NEEDED_SEXUALITY_FOR_CHILD 100
#define MUTATION_RATE 50		// percent of possible mutations
#define MUTATION_MAX 5			// maximal change of values through mutation

#define LOWER_HUNGER 70
#define LOWER_SOZIAL 100

#define DIRT_TO_PLANT 10		// dirt to plant (3<)

// enum for sheep states
enum SheepIntention{IDLE=0,		// do nothing
					HUNGER=1,	// hungry
					SOZIAL=2,	// needs social contact
					HYGIENE=3,	// needs to go to toilet
					SEXUALITY=4,// needs other sex sheep
					COMBAT=5};	// fight other sheep
// all possible sheep states

// board dimension
#define LEVEL_WIDTH 15
#define LEVEL_HEIGHT 12

// HEXAGON
#define NEIGHBOURS 6

// map
#define TILES_SCALE_FACTOR 1
#define TILES_SCALE_FACTOR_SMALL 0.5
#define TILES_SIZE_X 70
#define TILES_SIZE_Y 76
#define TILES_OFFSET_X 35
#define TILES_OFFSET_Y 57

// globals
static const int xofs[NEIGHBOURS] = { -1, 0, 0, -1, -1, 1 };
static const int xofsEven[NEIGHBOURS] = {0, 1, 1, -1, 0, 1};
static const int yofs[NEIGHBOURS] = { -1, -1, 1, 0, 1, 0 };	