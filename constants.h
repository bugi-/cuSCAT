#ifndef constants_h
#define constants_h

#define DIMS 3 // Number of dimensions

static inline float pow2(float x) {return x*x;}
static inline int pow2(int x) {return x*x;}
#define dot(x, y) ((x)[0]*(y)[0] + (x)[1]*(y)[1] + (x)[2]*(y)[2])
#define FLOAT_EPS (1E-6) // For comparing floating point numbers

#define MAX_SCATTERS (int)1E0 // Maximum number of scatterings for a single photon packet
#define STEP 0.1f // Step size for the photon packets. The value is relative to length of an element in the grid.
#define DETECTION_DIRECTION_LIMIT 0.1 // How many radians from a main axis the ray can point to be detected

// Status of the packet after simulating it.
#define RAY_OUT 1
#define RAY_IN 0

// Choice of random number generator
#define RNG_SEED(x) srand(x)
#define RNG(x) ((float)rand() / RAND_MAX)
#define SEED 1988 // Seed for RNG

#define OUTPUT_FILE "map.out"
#define OUTPUT_FILE_SEP " " // Separator for the output file

#endif
