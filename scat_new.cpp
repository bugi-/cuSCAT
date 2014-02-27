#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "types.h"
#include "constants.h"

// Gets the current element of the cloud model.
inline float cloud_index(Cloud *cl, Ray *r) {
	int i, j, k;
	i = (int) r->position[0];
	j = (int) r->position[1];
	k = (int) r->position[2];
	return cl->cloud[i + j * cl->side_length + k * pow2(cl->side_length)];
}

double mean_scatters = 0.0;
int simulate_ray(Cloud *cloud) {
	Ray ray (cloud);
	int scatters = 0;
	for (scatters = 0; scatters < MAX_SCATTERS; scatters++) {
		for (float tau = -log(RNG()); tau > 0.0f;) { // Take steps until tau reaches 0.
	
			// update tau
			tau -= cloud_index(cloud, &ray);
			
			// Take a step
			for (int i=0; i<DIMS; i++) {
				ray.position[i] += ray.direction[i];
			}
			
			// Stop if the ray is out of bounds
			if (!ray.in_cloud(cloud)) return RAY_OUT;
		}
		// Scatter
	}
	mean_scatters += scatters;
	return RAY_IN;
}

int main(void) {
	const int num_photons = (int)1E4;
	
	RNG_SEED(SEED);

	// Initialize the cloud
	Uniform_cloud cloud (100, 20.0, 0.5, 0.0, 0.0);

	int sum = 0;
	for (int i=0; i<num_photons; i++) {
		sum += simulate_ray(&cloud);
	}
	printf("%i, %i\n", sum, num_photons);
	printf("%1.0e\n", mean_scatters / num_photons);
	return 0;
}

