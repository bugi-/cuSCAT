#include "cuScat.h"

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

