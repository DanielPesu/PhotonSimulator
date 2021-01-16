#ifndef PHOTON_H_
#define PHOTON_H_
#include "positioning.h"  // for definition of 'coords' struct.

struct photon {
	double energy;	// The photon's current energy level in eV
	coords pos;		// The photon's x and y position (relative to the center of the detector).
};

#endif  // PHOTON_H_
