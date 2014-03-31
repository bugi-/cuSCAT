#include "cuScat.h"

// Gets the current element of the cloud model.
float inline cloud_index(Cloud *cl, Ray *r) {
	int i, j, k;
	i = (int) r->position[0];
	j = (int) r->position[1];
	k = (int) r->position[2];
	return cl->cloud[i + j * cl->side_length + k * pow2(cl->side_length)];
}
/*
float inline calc_d(int face, Ray *ray) {
	float temp[DIMS];
	return 0.0;
}

// Returns delta tau of the traversal
void traverse_element(Cloud *cloud, Ray *ray) {
	float exit_point[DIMS];
	float dist;
	//modf

	

	ray->position[0] = exit_point[0];
	ray->position[1] = exit_point[1];
	ray->position[2] = exit_point[2];
	cloud_index(cloud, ray) * dist;
}
*/
// Variables for statistics
float mean_tau = 0.0f;
float mean_scatters = 0.0f;
float mean_intensity = 0.0f;
int simulate_ray(Cloud *cloud) {
	Ray ray (cloud);
	int scatters;
	for (scatters = 0; scatters < MAX_SCATTERS; scatters++) {
		float tau = -log(RNG());
		mean_tau += tau;
		while (tau > 0.0f) { // Take steps until tau reaches 0.
			// update tau
			tau -= cloud_index(cloud, &ray) * STEP;
			
			/*
			 *	Switch to analytical steps
			 */
			
			// Take a step
			for (int i=0; i<DIMS; i++) {
				ray.position[i] += ray.direction[i];
			}
			
			// Stop if the ray is out of bounds
			if (!ray.in_cloud(cloud)) {
				ray.process_output(cloud);
				mean_intensity += ray.intensity;
				return RAY_OUT;
			}
		}
		ray.scatter(cloud);
		mean_scatters += scatters;
	}
	mean_intensity += ray.intensity;
	return RAY_IN;
}

