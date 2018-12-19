#pragma once

#ifdef WIN32
#pragma warning(disable : 4786)
#endif

#define DEBUG 0

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600

// timer
#define TIMER 3000				// time till explosion in seconds

// particles		
#define MAX_LIFETIME_MAX 6		// particle lifetime in seconds
#define MAX_LIFETIME_MIN 4
#define PARTICLE_SIZE 3.0		// max. particle size
#define PARTICLEMASS 1.0		// 1/mass of particle

#define PARTICLE_NUMBER 100		// number of newly created particles
#define MAX_TIEFE 2				// number of explosions

// gravitation
#define DT 0.02
#define GRAVITY_EARTH 9.81		// earth gravitation
#define START_IMPULSE 70.0		// rocket impulse start
#define EXPLOSION_MIN 5			// explosion impulse
#define EXPLOSION_MAX 20

#define TIME_SCALING 0.001