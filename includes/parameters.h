#ifndef PARAMETERS_H_
#define PARAMETERS_H_
#include <fstream>

struct param {
	double containment_cube_length;		// The length of the containment cube in mm
	double detector_cube_length;		// The length of the germanium detector in mm

	double photon_energy_initial;		// Energy level of produced photons in eV
	double photon_energy_lower_lim;		// Cut-off limit for photons in eV

	double prob_absorbance;				// probability of a photon being absorbed by water over 1mm
	double prob_scatter;				// probability of a photon scattering over 1mm

	double energy_loss_max;				// max ratio of energy loss on a scatter
	double energy_loss_min;				// min ratio of energy loss on a scatter
};

// Function prototypes. Definitions in configure.cpp.
param readSimulationParameters();
bool createConfigFile();
double getNextParameter(std::ifstream* ins);

#endif  // PARAMETERS_H_
