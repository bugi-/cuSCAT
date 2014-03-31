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
	float albedo; // Albedo of a single particle.
	float g; // Asymmetry parameter for Heyney-Greenstein function
	float *cloud; // Optical depth per distance unit of each element in the cloud.
	float *map; // Map of scattered light.
	
	Cloud(int side, float albed, float gi) {
		side_length = side;
		side_length_f = float(side);
		step_size = STEP;
		albedo = albed;
		g = gi;
	}
};

// Cloud with a uniform (constant) density
class Uniform_cloud: public Cloud {
public:
	Uniform_cloud(int side, float albedo, float gi, float val) :
	Cloud(side, albedo, gi) {
		int size = side_length;
		int cloud_size = size*size*size;
		cloud = new float[cloud_size];
		map = new float[pow2(size)];
		for (int i = 0; i < cloud_size; i++) {
			cloud[i] = val;
		}
	}

	// Writes the map to the file as whitespace separated text
	void map2file(void) {
		ofstream file;
		file.open(OUTPUT_FILE);
		file.setf(ios::scientific);
		file.width(8);
		
		for (int i=0; i<side_length; i++) {
			for (int j=0; j<side_length; j++) {
				file << map[i + j*side_length] << OUTPUT_FILE_SEP;
			}
			file << endl;
		}
		
		file.close();
	}
	
	~Uniform_cloud() {
		delete[] cloud;
		delete[] map;
	}
};

// Constant density cloud where outside is zero.
class Uniform_sphere: public Uniform_cloud {
public:
	Uniform_sphere(int side, float albedo, float gi, float val) :
	Uniform_cloud(side, albedo, gi, val) {
	int size = side_length;
	float distance;
	float mid = side / 2.0f; // Midpoint of the cloud in all dimensions
	float fi, fj, fk; // Float versions of the loop variables with a following offset
	float offset = 0.5f;
		for (int i=0; i<size; i++) {
			fi = i + offset;
			for (int j=0; j<size; j++) {
				fj = j + offset;
				for (int k=0; k<size; k++) {
					fk = k + offset;
					distance = pow2(fi-mid);
					distance += pow2(fj-mid);
					distance += pow2(fk-mid);
					distance = sqrt(distance);
					if (distance > mid) {
						cloud[i + j * size + k * pow2(size)] = 0.0f;
					}
				}
			}
		}
	}
};

// A single photon packet
class Ray {
public:
	// Lists are ordered as x,y,z.
	float position[DIMS];
	float direction[DIMS];
	float dir_len; // Length of the direction vector
	float intensity;
	float g;
	bool zero_g; // Whether g is zero or not
	// Initialization to x,y-plane
	Ray(Cloud* params) {
		intensity = 1.0f;
		g = params->g;
		if (fabs(g) < FLOAT_EPS) zero_g = true;
		else zero_g = false;
		for (int i=0; i<DIMS-1; i++) {
			position[i] = RNG() * params->side_length_f;
		}
		position[DIMS-1] = 0.0f; // Last dimension is 0
		
		// Initialize the direction vector. In this case the direction vector is a unit vector times step_size in the z-direction.
		for (int i=0; i<DIMS-1; i++) {
			direction[i] = 0.0f * params->step_size;
		}
		direction[DIMS-1] = 1.0f * params->step_size;
		dir_len = sqrt(dot(direction, direction));
	}
	
	// Returns true if the ray is still in the cloud. Otherwise returns false.
	bool in_cloud(Cloud *cl) {
		float side = cl->side_length_f;
		if (position[0] < 0.0f || position[0] >= side) return false;
		if (position[1] < 0.0f || position[1] >= side) return false;
		if (position[2] < 0.0f || position[2] >= side) return false;
		return true;
	}
	
	// When the ray leaves cloud, see if it is captured by the observer. Only the z = side_length side is currently used in the map.
	void process_output(Cloud *cl) {
		float side = cl->side_length_f;
		float angle = fabs(acos(direction[2]/dir_len)); // Inside fabs is essentially projection of the direction vector on the z unit vector.
		if (position[2] >= side && angle < DETECTION_DIRECTION_LIMIT) {
			int subs = (int)position[0] + (int)position[1]*side;
			(cl->map)[subs] += intensity;
		}
	}

	// The following functions return the angle given by Henyey-Greenstein phase function in radians. The latter gives the angle for g = 0.
	static float HG(float g) {
		float ret = 1 + pow2(g);
		ret -= pow2((1-pow2(g)) / (1-g+2*g*RNG()));
		ret /= 2*g;
		return acos(ret);
	}
	
	static float HG0(void) {
		return acos(1.0f - 2.0f*RNG());
	}
	
	// Scatters the ray to a new direction
	void scatter(Cloud *cl) {
		// Calculate the scattering direction
		float phi = 2.0f*M_PI * RNG();
		float theta;
		if (zero_g) theta = HG0();
		else theta = HG(g);
		
		float new_direction[DIMS];
		new_direction[0] = sin(theta)*(direction[0]*direction[2]*cos(phi) - direction[1]*sin(phi));
		new_direction[0] /= sqrtf(1-pow2(direction[2]));
		new_direction[0] += direction[0]*cos(theta);
		
		new_direction[1] = sin(theta)*(direction[1]*direction[2]*cos(phi) + direction[0]*sin(phi));
		new_direction[1] /= sqrtf(1.0f-pow2(direction[2]));
		new_direction[1] += direction[1]*cos(theta);
		
		new_direction[2] = -sqrtf(1.0f-pow2(direction[2]))*sin(theta)*cos(phi)+direction[2]*cos(theta);

		direction[0] = new_direction[0];
		direction[1] = new_direction[1];
		direction[2] = new_direction[2];

		intensity *= cl->albedo;

		return;
	}
};

#endif
