#include "constants.h"
#include "parameters.h"
#include "photon.h"
#include "positioning.h"
#include "mathoperations.h"
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <iostream>
using namespace std;

extern const param params;
enum sim_type {NAIVE_SIMULATION, EFFICIENT_SIMULATION};

void simulate(unsigned int n, sim_type t);
photon generatePhoton();
double trackPhoton(photon p, double lifetime);
double trackPhoton(photon p);

void recordCoordinates(fstream& file, coords pos);
void prepareFile(fstream& file);

int main(int argc, char* argv[])
{
	srand(time(0));
	const clock_t t = clock();
	double computation_time;
	bool efficient_sim = true, naive_sim = true;  // Simulation Options.

	/* Verify command-line arguments */
  	if (argc < 3)
  	{
   	 cerr << "Usage: " << argv[0] << " [-E|-N] <NUMBER_OF_PHOTONS>" << endl;
   	 return 1;
  	}
  	else if (argc == 3)  // check for -E or -N tag.
 	{
     if (atoi(argv[2]) <	 10)
     {
       cerr << "ERROR: Invalid photon count! Must be at least 10." << endl;
       return 1;
     }
   	 if (argv[1][1] == 'N')
   	   efficient_sim = false;
   	 else if (argv[1][1] == 'E')
   	   naive_sim = false;
   	 else
   	 {
   	   cerr << "Unknown argument \'" << argv[1] << "\'." << endl;
   	   return 1;
   	 }
  	}	

	if (efficient_sim)
	{
		cout << "Performing efficient simulation.." << endl;
		simulate(atoi(argv[2]), EFFICIENT_SIMULATION);
	}
	else
	{
		cout << "Performing naive simulation.." << endl;
		simulate(atoi(argv[2]), NAIVE_SIMULATION);
	}
	computation_time = (double)(clock() - t)/CLOCKS_PER_SEC;

	cout << "Process took " << computation_time << " sec." << endl; 
	return 0;
}

//*******************************************************************************
//	Performs the simulation for a given number of photons.                      *
//*******************************************************************************
void simulate(unsigned int n, sim_type t)
{

	double detector_energy = 0.0, energy_deposited;
	int count = 0;
	coords center_of_beam = { 0, params.containment_cube_length/2, 0};

	cout << "The beam is " << params.containment_cube_length/2 - params.detector_cube_length/2 << " mm from the detector and is centered at (" << center_of_beam.x << ", " << center_of_beam.y << ", " << center_of_beam.z << ")" << endl;

	// Build Progress Bar. (Interestingly, breaks for counts < 10)
	cout << "Firing " << n << " photons.." << endl;
	cout << "[-----------]" << "\r[" << flush;

	for (unsigned int i = 0; i < n; ++i)
	{
		// Progress Indicator.
		// TODO: Fix for numbers which aren't exact multiples of 10
		if (!(i % (n/10)))
			cout << '*' << flush;

		// Fire and track a photon.
		if (t == EFFICIENT_SIMULATION)
			energy_deposited = trackPhoton(generatePhoton(), randomGeometric(params.prob_absorbance));
		else
			energy_deposited = trackPhoton(generatePhoton());

		// Increment total energy deposited.
		if (energy_deposited > 0.0)
		{
			++count;
			detector_energy += energy_deposited;
		}
	}

	cout << '*' << endl;
	cout << count << " photons (" << static_cast<double>(count)/n *100 << "%) reached the detector, depositing a total of " << detector_energy/1000000 << " MeV." << endl;
}


//******************************************************************************
// Spawns a photon randomly on the photon gun with initial energy.             *
//******************************************************************************
photon generatePhoton()
{
	coords point = randomPointDisk(params.detector_cube_length/10);
	photon p = {params.photon_energy_initial, {point.y, point.z+params.containment_cube_length/2, point.x}};

	return p;
}

//******************************************************************************
//	Performs efficient tracking of a photon throughout its lifetime.           *
//	At the end of its course its remaining energy is returned.                 *
//******************************************************************************
double trackPhoton(photon p, double lifetime)
{
	bool particle_terminated = false;
	double distance = randomGeometric(params.prob_scatter), total_distance = 0.0, deposited_energy = 0.0;
	coords new_pos = {p.pos.x, p.pos.y - distance, p.pos.z};  // coordinate of first collision.

	do
	{
		double t = didHitDetector(p.pos, new_pos);

		if (!(t < 0 || t > 1))
		{
			particle_terminated = true;

			// Check Partial Path length.
			if (total_distance + t*distance < lifetime)
				deposited_energy = p.energy;
		}
		
		if (isOutOfBounds(new_pos))
			particle_terminated = true;

		// Advance photon to collision.
		p.pos = new_pos;
		total_distance += distance;
		p.energy = p.energy*(1 - randomBetween(params.energy_loss_min, params.energy_loss_max));

		// Create next path.
		distance = randomGeometric(params.prob_scatter);
		new_pos = newPos(p.pos, distance);
	} while (!particle_terminated && p.energy > params.photon_energy_lower_lim && total_distance < lifetime);

	return deposited_energy;
}

//******************************************************************************
//	Performs naive tracking of a photon.                                       *
//	The photon is incremented by 1mm at a time, at each point checking if      *
//		- A collision has occurred,                                            *
//		- The photon has been absorbed by the water,                           *
//		- The photon has escaped the containment box, or                       *
//		- The photon hit the detector.                                         *
//	In the event of the latter three, a flag is set indicating that the        *
//	particle is to be terminated.                                              *
//	At the end of its course the remaining energy (if any) is returned.        *
//******************************************************************************
double trackPhoton(photon p)
{
	bool particle_terminated = false;
	double deposited_energy = 0.0;
	coords prev_pos = p.pos;

	p.pos.y = p.pos.y -1;  // Travel 1mm from photon gun.
	do
	{
		// Check Events.
		if (randomBetween(0.0, 1.0) < params.prob_absorbance)
			particle_terminated = true;  // Photon absorbed by water.
		else if (isOutOfBounds(p.pos))
			particle_terminated = true;  // Photon escaped containment.
		else if (didHitDetector(p.pos))
		{
			// Photon hit detector.
			particle_terminated = true;
			deposited_energy = p.energy;
		}
		else
		{
			// Check for collision.
			if (randomBetween(0.0, 1.0) < params.prob_scatter)
			{
				prev_pos = p.pos;
				p.pos = newPos(p.pos, 1);
				p.energy = p.energy*(1 - randomBetween(params.energy_loss_min, params.energy_loss_max));
			}
			else
			{
				// Traverse 1mm in current direction.
				coords vector = {p.pos.x-prev_pos.x, p.pos.y-prev_pos.y, p.pos.z-prev_pos.z};
				prev_pos = p.pos;
				p.pos = traverseDistance(p.pos, vector, 1);
			}
		}

	} while (!particle_terminated && p.energy > params.photon_energy_lower_lim);

	return deposited_energy;
}
