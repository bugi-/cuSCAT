#include "constants.h"
#include <stdlib.h>

// Base class for other cloud types. Includes only parameters of the simulation grid.
class Cloud {
public:
	int side_length; // Number of elements per one side of the grid
	float side_length_f; // Same as float.
	float step_length; // Length of a single step of the ray.
	float el_side_length; // Length of a side for a single element
	float albedo; // Albedo of a single particle.
	float kappa;
	float *cloud;
	
	Cloud(int side, float el_side, float albed, float kapp) {
		side_length = side;
		side_length_f = float(side);
		el_side_length = el_side;
		step_length = el_side * STEP;
		albedo = albed;
		kappa = kapp;
	}
};

class Uniform_cloud: public Cloud {
public:
	Uniform_cloud(int side, float el_side, float albedo, float kappa, int size, float val) :
		Cloud(side, el_side, albedo, kappa) {
		size_t cloud_size = size*size*size*sizeof(float);
		float *cloud = (float *)malloc(cloud_size);
		for (int i = 0; i < cloud_size; i++) {
			cloud[i] = val;
		}
	}
};


// A single photon packet
class Ray {
public:
	// Lists are ordered as x,y,z.
	float position[DIMS];
	float direction[DIMS];
	float intensity;

	// Initialization to x,y-plane
	Ray(Cloud* params) {
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
};
