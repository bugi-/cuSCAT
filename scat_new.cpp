#include <stdlib.h>
#include <stdio.h>

#include "types.h"
#include "constants.h"

int main(void) {
	const int num_photons = (int)1E6;
	
	RNG_SEED(SEED);

	// Initialize the cloud
	Uniform_cloud cloud (100, 20.0, 0.5, 0.0, 0.5);

	for (int i=0; i<num_photons; i++) {
		Ray current_ray (&cloud);
	}
	return 0;
}

