#include "constants.h"
#include "mathoperations.h"
#include "parameters.h"
#include "positioning.h"
#include <cmath>
#include <cstdlib>
#include <iostream>
using namespace std;

extern const param params;

//******************************************************************************
//	Generates a random orientation in spherical coordinates and gets the       *
//	expected travel distance. These are added to the current coordinates and   *
//	the new position is returned.                                              *
//******************************************************************************
coords newPos(coords current, double r)
{
	// Generate a random orientation and distance in spherical coordinates.
	double theta = randomBetween(0, 2*M_PI);
	double phi = randomBetween(0, M_PI);
	coords new_pos;

	// Convert Spherical coordinates to cartesian and add to existing position.
	new_pos.x = current.x + r*cos(theta)*sin(phi);
	new_pos.y = current.y + r*sin(theta)*sin(phi);
	new_pos.z = current.z + r*cos(phi);

	return new_pos;
}

//******************************************************************************
//	Checks if the given coordinate is out of the bounds of the container.      *
//	If so, true is returned.                                                   *
//******************************************************************************
bool isOutOfBounds(coords current)
{
	if (abs(current.x) > (params.containment_cube_length/2) ||
		abs(current.y) > (params.containment_cube_length/2) ||
		abs(current.z) > (params.containment_cube_length/2))
		return true;
	else
		return false;
}

//******************************************************************************
//	This function determines if the detector was hit between to coordinates.   *
//	It is achieved by the following                                            *
//		1. Find the vector parameterisation of the line segment connecting the *
//		   two points.                                                         *
//		2. Computing the parameterised values of the infinite line segment     *
//         which intersect with the six planes parallel to the six faces of    *
//		   the detector.                                                       *
//		3. Any valid parameterisation is then further analysed to see if the   *
//		   intersection occurs on the bounded face of the detector. The first  *
//		   point for which this occurs is the coordinate of the intersection.  *
//******************************************************************************
double didHitDetector(coords from, coords to)
{
	double* t;							// The array of parameterisations.
	double T = -1.0;					// The returned parameterisation giving the intersection.
	bool found_intersection = false;	// Flag indicating if an intersection is found.
	coords direction_vector;			// The directional vector between the two given coordinates.
	int face_index = 0;

	face faces[] = {FRONT, RIGHT, TOP, LEFT, BOTTOM, REAR};

	// Compute the vector between the two points
	direction_vector.x = to.x - from.x;
	direction_vector.y = to.y - from.y;
	direction_vector.z = to.z - from.z;

	// Compute the parameterised values where intersections occur between the parameterised
	// line segment and the infinitely extended faces of the detector.
	t = parameterisations(from, direction_vector);

	while (!(found_intersection) && face_index < 6)
	{
		// Get next valid parameterisation.
		face_index = indexNext(t);

		if (face_index < 6)
		{
			// Determine if intersection occurs on the finite face of the detector.
			coords intersection_point = traverseRatio(from, direction_vector, t[face_index]);
			if (didHitDetector(intersection_point))
				found_intersection = true;
			else
				t[face_index] = -1.0;
		}
		else
			t[face_index] = -1.0;
	}

	if (found_intersection)
		T = t[face_index];

	delete [] t;
	return T;
}

//******************************************************************************
//	Returns true if the current coordinates are within or on the boundary of   *
//	the detector.                                                              *
//******************************************************************************
bool didHitDetector(coords pos)
{
	return (abs(pos.x) <= params.detector_cube_length/2 &&
	        abs(pos.y) <= params.detector_cube_length/2 &&
	        abs(pos.z) <= params.detector_cube_length/2);
}
