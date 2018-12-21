#pragma once

#ifdef WIN32
#pragma warning(disable : 4786)
#endif

#define DEBUG 0

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600
#define FLOOR 580

//Timer
#define TIMER 4000				// Explosion

//Partikel		
#define MAX_LIFETIME_MAX 50		// Lebenszeit eines Partikels in Sekunden
#define MAX_LIFETIME_MIN 34
#define PARTICLE_SIZE 9.0		// maximale Partikelgröße
#define PARTICLEMASS 1.0		// 1/Masse der Partikel

#define PARTICLE_NUMBER 10		// Anzahl neu erzeugter Partikel
#define MAX_TIEFE 2				// Anzahl der Explosionen

//Gravitation
#define DT 0.02
#define GRAVITY_EARTH 9.81		// Erdanziehungskraft
#define START_IMPULSE 50.0		// Impuls beim Start
#define EXPLOSION_MIN 5			// Impuls der Explosion
#define EXPLOSION_MAX 20

//Dämpfung
#define DECAY 0.7					// Dämpfung pro Kollision mit Ebene
#define MIN_DECAY 0.35
#define LINE_ANGLE 0.26179938779915	// Winkel der Ebene (15°)
#define PI_HALF	1.5707963267949		// PI / 2

#define TIME_SCALING 0.001

//Reibung Materialkonstanten
#define ICE_KONST 3
#define STEEL_KONST 0.5