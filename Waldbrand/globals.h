#pragma once

#define DEBUG 0

#define WINDOW_HEIGHT 500
#define WINDOW_WIDTH  500

#define WOOD_WIDTH 500
#define WOOD_HEIGHT 500

#define MIN 0
#define MAX 10000
#define PROPABILITY_WOOD 4500		// possibility for wood
#define PROPABILITY_FIRE 10			// possibility for fire

#define PROPABILITY_NEW_WOOD 100	// percent if new field is wood
#define PROPABILITY_NEW_FIRE 5		// percent if new field is fire (flying sparks)

enum Fieldvalue { EMPTY = 0, WOOD = 1, FIRE = 2 };