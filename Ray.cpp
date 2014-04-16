#include "cuScat.h"
#include "Cloud.cpp"

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
	static int HG_calls;
	static int HG0_calls;
	// Initialization to x,y-plane
	Ray(Cloud* params) {
		intensity = 1.0f;
		g = params->g;
		if (fabs(g) < FLOAT_EPS) zero_g = true;
		else zero_g = false;
		printf("g:%f, zero_g %d\n", g, zero_g);
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
		HG_calls++;
		float ret = 1.f + pow2(g);
		ret -= pow2((1.f-pow2(g)) / (1.f-g+2.f*g*RNG()));
		ret /= 2.f*g;
		return acos(ret);
	}
	
	static float HG0(void) {
		HG0_calls++;
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
// Initialize static class members
int Ray::HG_calls = 0;
int Ray::HG0_calls = 0;
