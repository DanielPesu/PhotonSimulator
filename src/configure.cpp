#include "parameters.h"
#include <fstream>
using namespace std;

//******************************************************************************
//	Opens the config file and reads the parameters from it in the expected     *
//  order. Incomplete config files will cause runtime errors.                  *
//  In the event of a missing config file a new one will be created with the   *
//  default values. If this new file cannot be created, an exception is        *
//  thrown.                                                                    *
//******************************************************************************
param readSimulationParameters()
{
	param simulation_parameters;
	ifstream config_stream("config.txt");

	// Check config file exists.
	if (!config_stream.is_open())
	{
		if (createConfigFile())
			config_stream.open("config.txt");
		else
			throw ("Error creating config file.");
	}

	// Set parameters for simulation.
	simulation_parameters.containment_cube_length = getNextParameter(&config_stream);
	simulation_parameters.detector_cube_length = getNextParameter(&config_stream);
	simulation_parameters.photon_energy_initial = getNextParameter(&config_stream);
	simulation_parameters.photon_energy_lower_lim = getNextParameter(&config_stream);
	simulation_parameters.prob_absorbance = getNextParameter(&config_stream);
	simulation_parameters.prob_scatter = getNextParameter(&config_stream);
	simulation_parameters.energy_loss_max = getNextParameter(&config_stream);
	simulation_parameters.energy_loss_min = getNextParameter(&config_stream);

	return simulation_parameters;
}

//******************************************************************************
//	Creates a config file in the current directory and fills it with the       *
//	default values for the simulation.                                         *
//	This function is usually called when the config file has been deleted.     *
//	If the config file could not be created, flase is returned.                *
//******************************************************************************
bool createConfigFile()
{
	ofstream config_stream;
	config_stream.open("config.txt", ios::trunc);

	// Check if stream successfully opened.
	if (!config_stream.is_open())
		return false;

	// Input data into the config file.
	config_stream	<< "containment cube length (mm) = 1000\n"
					<< "detector cube length (mm) = 20\n\n"
					<< "photon energy (eV) = 1000000\n"
					<< "lower energy limit (eV) = 1000\n\n"
					<< "probability of absorbance = 0.01\n"
					<< "probability of scattering = 0.05\n\n"
					<< "maximum energy loss percentage = 0.2\n"
					<< "minimum energy loss percentage = 0.0";

	// Close the stream.
	config_stream.close();

	return true;
}

//******************************************************************************
//	Returns the next parameter in the configuration file stream.               *
//******************************************************************************
double getNextParameter(ifstream* ins)
{
	double parameter = 0.0;

	// Skip to '=' character.
	while (ins->get() != '=') {}

	// Read value.
	*ins >> parameter;

//	cout << "Read parameter: " << parameter << endl;

	return parameter;
}
