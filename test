#include "positioning.h"
#include "constants.h"
#include <iostream>
#include <string>
using namespace std;

int main()
{
	coords test_point_from[] = {{0, params.containment_cube_length/2, 0},   // FRONT
	                            {-params.containment_cube_length/2, 0, 0},  // RIGHT
	                            {0, 0, params.containment_cube_length/2},   // TOP
	                            {params.containment_cube_length/2, 0, 0},   // LEFT
	                            {0, 0, -params.containment_cube_length/2},  // BOTTOM
	                            {0, -params.containment_cube_length/2, 0}   // REAR
	                           };

	coords test_point_to[] = {{0, test_point_from[0].y - (test_point_from[0].y - params.detector_cube_length/2), 0},     // FRONT
	                          {-(test_point_from[1].x - (test_point_from[1].x - params.detector_cube_length/2)), 0, 0},  // RIGHT
	                          {0, 0, test_point_from[2].z - (test_point_from[2].z - params.detector_cube_length/2)},     // TOP
	                          {test_point_from[3].x - (test_point_from[3].x - params.detector_cube_length/2), 0, 0},    // LEFT
	                          {0, 0, -(test_point_from[4].z - (test_point_from[4].z - params.detector_cube_length/2))},  // BOTTOM
	                          {0, -(test_point_from[5].y - (test_point_from[5].y - params.detector_cube_length/2)), 0}   // REAR
	                         };

	coords test_point_opposite[] = {{0, -test_point_from[0].y, 0},
	                                {-test_point_from[1].x, 0, 0},
	                                {0, 0, -test_point_from[2].z},
	                                {-test_point_from[3].x, 0, 0},
	                                {0, 0, -test_point_from[4].z},
	                                {0, -test_point_from[5].y, 0},
	                               };

	string face[] = {"FRONT", "RIGHT", "TOP", "LEFT", "BOTTOM", "REAR"};

	cout << "Detector Size: " << params.detector_cube_length << " mm" << endl;
	cout << "Container Size: " << params.containment_cube_length << " mm" << endl;

	cout << "Testing Boundaries.." << endl;
	for (int i = 0; i < 6; ++i)
	{

		cout << "   " << face[i] << ": ";
		coords V = {test_point_to[i].x - test_point_from[i].x, test_point_to[i].y - test_point_from[i].y, test_point_to[i].z - test_point_from[i].z};
		coords V0 = test_point_from[i];	

/*
		cout << "Firing Photon from "
		     << " (" << test_point_from[i].x << ", " << test_point_from[i].y << ", " << test_point_from[i].z << ")"
		     << " to (" << test_point_to[i].x << ", " << test_point_to[i].y << ", " << test_point_to[i].z << ")" << endl;
		cout << "  Line parameterisation is (" << V0.x << ", " << V0.y << ", " << V0.z << ") + t(" << V.x << ", " << V.y << ", " << V.z << ")" << endl;
*/


		double t = didHitDetector(test_point_from[i], test_point_to[i]);

		if (!(t < 0))
		{
			if (t == 1)
				cout << "PASS." << endl;
			else
				cout << "FAIL. Hit detector at (" << V0.x + t*V.x << ", " << V0.y + t*V.y << ", " << V0.z + t*V.z << ")" << endl;
		}
		else
		{
			cout << "FAIL. Photon missed detector." << endl;
		}
	}

	cout << "\nTesting Face Resolution.." << endl;
	double target = (params.containment_cube_length-params.detector_cube_length)/200;
	for (int i = 0; i < 6; ++i)
	{
		cout << "   " << face[i] << ": ";

		double t = didHitDetector(test_point_from[i], test_point_opposite[i]);

		if (t != target)
		{
			cout << "Fail. Expected t=" << target << " but recieved " << t << endl;
		}
		else
			cout << "PASS." << endl;
	}
	
	return 0;
}
