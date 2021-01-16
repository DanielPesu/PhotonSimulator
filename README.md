# PhotonSimulator
A program designed to simulate the path of photons through a body of water into a detector.

Contents:
	1. Usage
	2. Simulation Assumptions

-----------------------------------------------
  1. Usage
-----------------------------------------------
To compile the source code and build the program, type

	$ make Photon_Deposition

into the command line from the root directory. (the file where you can see src, Includes, makefile, etc. from)

The build will then be compiled, if not, ensure you are in the correct directory, and that the parameters in
Includes/constants.h are default values.

To run the program simply enter into the commandline the run command, ./Photon_Deposition, followed by either a
-E or -N tag to indicate either an efficient or naive simulation method, followed by the number of photons to simulate.
In command line logic, this is

	$ ./Photon_Deposition [-N|-E] <NUMBER_OF_PHOTONS>

At any point, you can type ./Photon_Deposition on its own, or incorrectly (to an extent, you may break it if you're
being stupid ie. negative photons), to see it's usage.

The simulation will then run the simulation. A progress indicator will indicate how far along it is (although it has
broken before for really large simulations. In such situations, the simulation is still running, just the progress
is incorrectly reported). Once it is complete it will report some basic stats.

-----------------------------------------------
  2. ASSUMPTIONS
-----------------------------------------------
The following package contains a computer program designed for simulating the amount of energy deposited into
a germanium detector after passing through a barrier of water.

The parameters for the simulation are entered into a text file named config.txt ** NOTE THIS IS CURRENTLY BROKEN **
	parameters need to be hard-coded into the 'params' variable in Includes/constants.h

 - The effects of each photon are independent and do not interact with each other.

 - The photons are produced uniformly from a single circular source of given radius,
   and travel perpendicularly to the surface of the detector.

 - Both the detector and the encompassing water barrier are cubes.

 - Every millimeter of travel, a photon has a constant probability of being absorbed
   in the water, and a constant probability of being scattered.

 - Once a photon hits the detector, it's energy is fully absorbed. If a photon leaves
   the containing water cube, it cannot re-enter.
