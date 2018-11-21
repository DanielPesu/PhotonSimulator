#include "constants.h"
#include "mathoperations.h"
#include "positioning.h"
#include <cstdlib>  // rand()
#include <cmath>

extern const param params;

//******************************************************************************
//	Generates a uniform random variable and uses it to create a random         *
//  geometric variable with probability of 'success' p.                        *
//******************************************************************************
int randomGeometric(double p)
{
	return floor(log(static_cast<double>(rand())/RAND_MAX)/log(1-p));
}

//******************************************************************************
//	Returns the solution to a simulatneous equation of the form                *
//		A + B = X                                                              *
//		A - B = Y                                                              *
//  given the constants X and Y.                                               *
//  Returns the constants A and B as a pair.                                   *
//******************************************************************************
Pair solveSimultaneous(double X, double Y)
{
	Pair solution;

	solution.A = X-(X-Y)/2;
	solution.B = (X-Y)/2;

	return solution;
}

//******************************************************************************
//	Returns the parameterisations at which the parameterised form of the line  *
//			X = V0 + t*V                                                       *
//	connects with each of the six infinite planes parallel to the six faces of *
//	the detector.                                                              *
//	The parameterisations are returned in the order of:                        *
//		Front Face, Right Face, Top Face, Left Face, Bottom Face, Rear Face    *
//                                                                             *
//	Any line segments which run parallel to and, hence, never touch a face are *
//	given a parameterisation of t = -1.                                        *
//	The array is allocated dynamically, so it will need to be deleted when     *
//  done.                                                                      *
//******************************************************************************
double* parameterisations(coords V0, coords V)
{
	double* t = new double[6]();
	double k = params.detector_cube_length/2;  // An off-set for scaling the cube's faces.

	if (V.x != 0.0)
	{
		// Right Face (-k,0,0)
		t[1] = (-k - V0.x)/V.x;

		// Left Face (k,0,0)
		t[3] = (k - V0.x)/V.x;
	}
	else
		t[1] = t[3] = -1;

	if (V.y != 0.0)
	{
		// Front Face (0,k,0)
		t[0] = (k - V0.y)/V.y;

		// Rear Face (0,-k,0)
		t[5] = (-k - V0.y)/V.y;
	}
	else
		t[0] = t[5] = -1;

	if (V.z != 0.0)
	{
		// Top Face (0,0,k)
		t[2] = (k - V0.z)/V.z;

		// Bottom Face (0,0,-k)
		t[4] = (-k - V0.z)/V.z;
	}
	else
		t[2] = t[4] = -1;

	return t;
}

//******************************************************************************
//	Computes the coordinates where a point lies on a face, relative to that    *
//	face's basis vectors. Only the magnitudes of the pair is guaranteed to be  *
//	correct.                                                                   *
//******************************************************************************
/*** THERE WAS LITERALLY NO NEED FOR THIS **************************************
*Pair computeComponents(coords V0, coords V, double t, face f)
*{
*	Pair components;
*
*	switch (f)
*	{
*		case FRONT:
*		case REAR:
*			components = solveSimultaneous(V0.z + t*V.z, V0.x + t*V.x);
*			break;
*		case LEFT:
*		case RIGHT:
*			components = solveSimultaneous(V0.z + t*V.z, V0.y + t*V.y);
*			break;
*		case TOP:
*		case BOTTOM:
*			components = solveSimultaneous(V0.x + t*V.x, V0.y + t*V.y);
*			break;
*	}
*
*	return components;
*}
********************************************************************************/

//******************************************************************************
//	Generates random points on the disk of given radius.                       *
//******************************************************************************
coords randomPointDisk(double radius)
{
	coords point;
	double r = randomBetween(0, pow(radius, 2.0));
	double theta = randomBetween(0, 2*M_PI);

	point.x = sqrt(r)*cos(theta);
	point.y = sqrt(r)*sin(theta);
	point.z = 0;

	return point;
}

//******************************************************************************
//	Returns the point that is a set distance along a given vector.             *
//******************************************************************************
coords traverseDistance(coords current, coords vector, double distance)
{
	double magnitude = sqrt(pow(vector.x,2.0)+pow(vector.y,2.0)+pow(vector.z,2.0));

	coords new_pos = {current.x + (vector.x/magnitude)*distance, current.y + (vector.y/magnitude)*distance, current.z + (vector.z/magnitude)*distance};

	return new_pos;
}

//******************************************************************************
//	Returns the point from the parameterised line.... (Explain gooder).
//******************************************************************************
coords traverseRatio(coords current, coords vector, double t)
{
	coords new_pos = {current.x + vector.x*t, current.y + vector.y*t, current.z + vector.z*t};

	return new_pos;
}

// Returns a random number between two limits.
double randomBetween(double lower_lim, double upper_lim)
{
  return lower_lim + static_cast<double> (rand()) / (static_cast<double>(RAND_MAX)/(upper_lim - lower_lim));
}

int indexNext(double* t)
{
	int lowest_index = 0;
	while ((t[lowest_index] < 0 || t[lowest_index] > 1) && lowest_index < 6)
		++lowest_index;

	for (int i = lowest_index+1; i < 6; ++i)
		if (t[i] < t[lowest_index] && !(t[i] < 0) && !(t[i] > 1))
			lowest_index = i;
		
	return lowest_index;
}


