#include "scat.h"
#include "scattering.c"

int main(void) {
	const int num_photons = (int)1E4;
	extern double mean_scatters;
	RNG_SEED(SEED);

	// Initialize the cloud
	Uniform_cloud cloud (100, 20.0, 0.5, 0.0);

	int sum = 0;
	for (int i=0; i<num_photons; i++) {
		sum += simulate_ray(&cloud);
	}
	printf("%i, %i\n", sum, num_photons);
	printf("%1.0e\n", mean_scatters / num_photons);
	return 0;
}

