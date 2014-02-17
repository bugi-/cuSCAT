#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "types.h"
#include "constants.h"

int simulate_ray(Cloud *cloud) {
	Ray current_ray (cloud);
	for (float tau = -log(RNG()); tau > 0.0f; tau -= cloud) { // Take steps until tau reaches 0.

		// update tau

		// Take a step
		for (int i=0; i<DIMS; i++) {
			current_ray.position[i] += current_ray.direction[i];
		}
		
	}
	return RAY_OUT;
}

int main(void) {
	const int num_photons = (int)1E6;
	
	RNG_SEED(SEED);

	// Initialize the cloud
	Uniform_cloud cloud (100, 20.0, 0.5, 0.0, 0.5);

	for (int i=0; i<num_photons; i++) {
		simulate_ray(&cloud);
	}
	return 0;
}

