#include <stdlib.h>
#include <stdio.h>

#include "types.h"
#include "constants.h"

// Returns a cloud of size^3 with constants density of val.
float* generate_constant_density_cloud(int size, float val) {
	size_t cloud_size = size*size*size*sizeof(float);
	float *cloud = (float *)malloc(cloud_size);
	for (int i = 0; i < size*size*size; i++) {
		cloud[i] = val;
	}
	return cloud;
}

int main(void) {
	const int num_photons = (int)1E6;

	for (int i=0; i<num_photons; i++) {
		
	}
	return 0;
}

