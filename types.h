#include "constants.h"
#include <stdlib.h>
// Parameters of the simulation grid
typedef struct {
	int side_length; // Number of elements per one side of the grid
	float side_length_f; // Same as float.
	float step_length; // Length of a single step of the ray.
	float el_side_length; // Length of a side for a single element
	float albedo; // Albedo of a single particle.
	float kappa;
} cloud_params;


// A single photon packet
class Ray {
public:
	// Lists are ordered as x,y,z.
	float position[DIMS];
	float direction[DIMS];
	float intensity;
	Ray(cloud_params*);
};
// Initializes a photon packet on the x,y-plane
Ray::Ray(cloud_params* params) {
	intensity = 1.0f;
	for (int i=0; i<DIMS-1; i++) {
		position[i] = RNG() * params->side_length_f;
	}
	position[DIMS-1] = 0.0f; // Last dimension is always 0
	
	// Initialize the direction vector. In this case the direction vector is a unit vector in the z-direction.
	for (int i=0; i<DIMS-1; i++) {
		direction[i] = 0.0f * STEP;
	}
	direction[DIMS-1] = 1.0f * STEP;
}
