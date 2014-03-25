#ifndef types_h
#define types_h

#include "cuScat.h"
#include "constants.h"

using namespace std;

// Base class for other cloud types. Includes only parameters of the simulation grid. Some values are precomputed for faster access later.
class Cloud {
public:
	int side_length; // Number of elements per one side of the grid
	float side_length_f; // Same cast as float for easier access.
	float step_size; // Length of a single step of the ray.
	float el_side_length; // Length of a side for a single element
	float albedo; // Albedo of a single particle.
	float *cloud; // Optical depth per distance unit of each element in the cloud
	
	Cloud(int side, float el_side, float albed) {
		side_length = side;
		side_length_f = float(side);
		el_side_length = el_side;
		step_size = el_side * STEP;
		albedo = albed;
	}
};

// Cloud with a uniform (constant) density
class Uniform_cloud: public Cloud {
public:
	Uniform_cloud(int side, float el_side, float albedo, float val) :
	Cloud(side, el_side, albedo) {
		int size = side_length;
		int cloud_size = size*size*size;
		cloud = new float[cloud_size];
		for (int i = 0; i < cloud_size; i++) {
			cloud[i] = val;
		}
//		cout << "" << cloud[0] << " " << cloud[15] << endl;
		printf("%f %f \n", cloud[0], cloud[15]);
	}
	
	~Uniform_cloud() {
		delete[] cloud;
	}
};


// A single photon packet
class Ray {
public:
	// Lists are ordered as x,y,z.
	float position[DIMS];
	float direction[DIMS];
	float intensity;

	Ray(void) {}

	// Initialization to x,y-plane
	Ray(Cloud* params) {
		intensity = 1.0f;
		for (int i=0; i<DIMS-1; i++) {
			position[i] = RNG() * params->side_length_f;
		}
		position[DIMS-1] = 0.0f; // Last dimension is 0
		
		// Initialize the direction vector. In this case the direction vector is a unit vector times step_size in the z-direction.
		for (int i=0; i<DIMS-1; i++) {
			direction[i] = 0.0f * params->step_size;
		}
		direction[DIMS-1] = 1.0f * params->step_size;
	}
	
	// Returns true if the ray is still in the cloud. Otherwise returns false.
	bool in_cloud(Cloud *cl) {
		if (position[0] < 0.0f || position[0] >= cl->side_length_f) return false;
		if (position[1] < 0.0f || position[1] >= cl->side_length_f) return false;
		if (position[2] < 0.0f || position[2] >= cl->side_length_f) return false;
		return true;
	}
	
};

#endif
