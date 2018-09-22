#pragma once

// debug mode on (1) or off (0) 
#define DEBUG 1

// board dimension
#define LEVEL_WIDTH 7
#define LEVEL_HEIGHT 6

// window size
#define WINDOW_WIDTH 560
#define WINDOW_HEIGHT 500

// cursor
#define CURSOR_SCALE_FACTOR	2
#define CURSOR_SIZE	80
#define CURSOR_OFFSET 17			// cursor position middle

// map
#define TILES_SCALE_FACTOR 2
#define TILES_SIZE 80
#define MAP_OFFSET 20				// render map below cursor

// AI
#define TREE_DEEP 6					// max deep of precomputed gametree
#define AI_COLOR YELLOW
#define PLAYER_COLOR RED
// WEIGHTFACTORS
#define WF1  10000
#define WF2  1000
#define WF3  100
#define WF4  10
#define WFE1 50000
#define WFE2 5000
#define WFE3 500
#define WFE4 50

// enum for board field states
enum Fieldvalue { EMPTY = 0, RED = 1, YELLOW = 2 };