#ifndef POSITIONING_H_
#define POSITIONING_H_

// In the simulation, coordinates are in cartesian form with the origin set to the center of the detector.
struct coords {
	double x;
	double y;
	double z;
};

enum face {FRONT = 0, RIGHT, TOP, LEFT, BOTTOM, REAR};

coords newPos(coords current, double r);
bool isOutOfBounds(coords current);
double didHitDetector(coords from, coords to);
bool didHitDetector(coords pos);

#endif  // POSITIONING_H_
