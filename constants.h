#ifndef constants_h
#define constants_h

#define DIMS 3 // Number of dimensions

#define SEED 1988 // Seed for RNG

#define MAX_SCATTER (int)1E2 // Maximum number of scatterings for a single photon packet
#define STEP 0.1f // Step size for the photon packets. The value is relative to length of an element in the grid.

// Status of the packet after simulating it.
#define RAY_OUT 0
#define RAY_IN 1

// Choice of random number generator
#define RNG_SEED(x) srand(x)
#define RNG(x) rand()


#endif
