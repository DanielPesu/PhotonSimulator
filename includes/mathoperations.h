#ifndef MATHOPERATIONS_H_
#define MATHOPERATIONS_H_
#include "positioning.h"

// Used to return the pair of solutions to a simultaneous equation.
struct Pair {
	double A;
	double B;
};


int randomGeometric(double p);
coords randomPointDisk(double radius);
Pair solveSimultaneous(double A, double B);
double* parameterisations(coords V0, coords V);
//Pair computeComponents(coords V0, coords V, double t, face f); Obsolete
double randomBetween(double lower_lim, double upper_lim);
int indexNext(double* t);
coords traverseDistance(coords current, coords vector, double distance);
coords traverseRatio(coords current, coords vector, double t);

#endif  // MATHOPERATIONS_H_
