#include "cuScat.h"
#include "scattering.c"

int main(void) {
	const int num_rays = (int)1E6;
	//extern float mean_scatters;
	clock_t start, end;
	
	start = clock();
	
	
	RNG_SEED(SEED);

	// Initialize the cloud
	                    //size, albedo, g, val
	Uniform_sphere cloud (100, 0.5, -1.0, 1.E-2);

	int sum = 0;
	for (int i=0; i<num_rays; i++) {
		sum += simulate_ray(&cloud);
	}
	cloud.map2file();
	printf("Rays out, total: %i, %i\n", sum, num_rays);
	printf("mean scatters: %1.0e\n", mean_scatters / num_rays);
	printf("mean tau: %3.2e\n", mean_tau / num_rays);
	printf("mean intensity: %3.2e\n", mean_intensity / num_rays);
	
	end = clock();
	printf("Time taken by program: %2.4fs\n", (double)(end-start)/(double)CLOCKS_PER_SEC);
	return 0;
}

