#ifndef constants_h
#define constants_h

#define DIMS 3 // Number of dimensions

#define pow2(x) ((x)*(x))

#define MAX_SCATTERS (int)1E3 // Maximum number of scatterings for a single photon packet
#define STEP 1.0f // Step size for the photon packets. The value is relative to length of an element in the grid.

// Status of the packet after simulating it.
#define RAY_OUT 0
#define RAY_IN 1

// Choice of random number generator
#define RNG_SEED(x) srand(x)
#define RNG(x) rand()
#define SEED 1988 // Seed for RNG


#endif
