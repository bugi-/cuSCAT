#include "cuScat.h"
#include "scattering.c"

int main(void) {
	const int num_rays = (int)1E5;
	extern double mean_scatters;
	RNG_SEED(SEED);

	// Initialize the cloud
	                    //size, albedo, g, val
	Uniform_sphere cloud (100, 0.5, 0.0, 100.0);

	int sum = 0;
	for (int i=0; i<num_rays; i++) {
		sum += simulate_ray(&cloud);
	}
	cloud.map2file();
	printf("Rays out, total: %i, %i\n", sum, num_rays);
	printf("mean scatters: %1.0e\n", mean_scatters / num_rays);
	printf("mean tau: %3.2e\n", mean_tau / num_rays);
	return 0;
}

