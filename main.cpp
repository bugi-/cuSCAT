#include "cuScat.h"
#include "scattering.c"

int main(void) {
	const int num_rays = (int)1E6;
	extern double mean_scatters;
	clock_t start, end;
	
	start = clock();
	
	
	RNG_SEED(SEED);

	// Initialize the cloud
	                    //size, albedo, g, val
	Uniform_sphere cloud (100, 0.5, 0.0, 1.E-2);

	int sum = 0;
	for (int i=0; i<num_rays; i++) {
		sum += simulate_ray(&cloud);
	}
	cloud.map2file();
	printf("Rays out, total: %i, %i\n", sum, num_rays);
	printf("mean scatters: %1.0e\n", mean_scatters / num_rays);
	printf("mean tau: %3.2e\n", mean_tau / num_rays);
	
	start = clock();
	printf("Time taken by program: %1.4fs\n", (double)(start-end)/(double)CLOCKS_PER_SEC);
	return 0;
}

