/*
Huom. k‰yt‰n C99 for-silmukoita, koska nvcc toimii C++-k‰‰t‰nj‰n‰, jolloin t‰m‰ ei luultavasti ole koskaan ongelma.
*/

#include <stdio.h>
#include <stdlib.h>

#define NUM_DIMS 3					// Simulaation dimensiot. Helpottaa muuttamista esim. 2d muotoon, mutta ei takaa sit‰ vain t‰t‰ muuttamalla. Ts. v‰hent‰‰ taikaluvun k‰yttˆ‰.
#define SEED 1988					// T‰m‰n avulla luodaan varsinaiset syˆtteet satunnaislukugeneraattoreille. "Master seed" tms.

#define MAX_SCATTER (int)1E2		// Suurin sallittu sirontojen m‰‰r‰ yhdelle fotonipaketille.
#define STEP 0.1f					// Kuinka suuri askel otetaan kerralla suhteessa yhden alkion kokoon, ts. sirontapaikkojen rakeisuus.
// Fotonipaketin tilan kuvaus simuloinnin lopussa
#define RAY_SUCCESS 0
#define RAY_TRAPPED 1

// Fotonipaketti
typedef struct {
	// Listat ovat j‰rjestyksess‰ x, y, z.
	float position[NUM_DIMS];
	float direction[NUM_DIMS];
	float intensity;
} ray;

// Mallinnettavan pilven ominaisuuksia
typedef struct {
	int side_length; // Alkioiden m‰‰r‰ yhdell‰ sivulla
	float side_length_f;
	float step_length; // Yhden siirtymisaskeleen pituus fysikaalisissa yksikˆiss‰
	float cell_side_length; // Solun sivun pituus metrein‰
	float albedo; // Yhden hiukkasen albedo
	float kappa;
} cloud_state;

cloud_state *state;


// Testailuun
void print_ray(ray *r) {
	printf("Intensity: %f\n", r->intensity);
	printf("Position: %f, %f, %f\n", r->position[0], r->position[1], r->position[2]);
	printf("Direction: %f, %f, %f\n", r->direction[0], r->direction[1], r->direction[2]);
}

// Alustaa fotonipaketin x,y-tasolla
ray* init_ray_planar(int *dims, curandStateMtgp32 *randStates) {
	ray *new_ray = (ray *)malloc(sizeof(ray)); // Muistivaraus tehd‰‰n laitteen muistiin (global memory). Voi hidastaa, mutta n‰m‰ j‰‰v‰t toivottavasti v‰limuistiin.
	// Alustetaan fotonit
	new_ray->intensity = 1.0f;
	// Arvotaan paikka x,y-tasossa
	int rand_cord = curand(&randStates[blockIdx.x]) % dims[0];
	if (threadIdx.x == 0) printf("%i\n", rand_cord);
	new_ray->position[0] = rand_cord + 0.5f; // Laitetaan kyseisen pinnan keskikohtaan
	rand_cord = curand(&randStates[blockIdx.x]) % dims[1];
	new_ray->position[1] = rand_cord + 0.5f;
	// Alustetaan paikka muissa dimensioissa
	new_ray->position[2] = 0.0f;
	
	// Alustetaan suunta (z-akselin suuntainen)
	new_ray->direction[0] = 0.0f * STEP;
	new_ray->direction[1] = 0.0f * STEP;
	new_ray->direction[2] = 1.0f * STEP;

	return new_ray;
}

// Alustaa fotonipaketin x,y-tasolla
ray* init_ray_planar2(int *dims, curandStateMtgp32 *randStates) {
	ray *new_ray = (ray *)malloc(sizeof(ray)); // Muistivaraus tehd‰‰n laitteen muistiin (global memory). Voi olla hidasta, mutta n‰m‰ j‰‰v‰t toivottavasti v‰limuistiin.
	// Alustetaan fotonit
	new_ray->intensity = 1.0f;
	// Arvotaan paikka x,y-tasossa
	float rand_cord = curand_uniform(&randStates[blockIdx.x]) * dims[0];
	new_ray->position[0] = rand_cord; // Laitetaan kyseisen pinnan keskikohtaan
	rand_cord = curand(&randStates[blockIdx.x]) * dims[1];
	new_ray->position[1] = rand_cord;
	// Alustetaan paikka muissa dimensioissa
	new_ray->position[2] = 0.0f;
	
	// Alustetaan suunta (z-akselin suuntainen)
	new_ray->direction[0] = 0.0f * STEP;
	new_ray->direction[1] = 0.0f * STEP;
	new_ray->direction[2] = 1.0f * STEP;

	return new_ray;
}

// Suorittaa s‰teilynkuljetuksen pilven sis‰ll‰. Palauttaa 0 mik‰li fotonipaketti siirtyy ulos simulaatioalueesta, paketin j‰‰dess‰ pilven sis‰‰n 1.
int simulate_ray(float *cloud, ray *cur_ray, curandStateMtgp32 *randStates) {
	for (int i = 0; i < MAX_SCATTER; i++) {
		float tau = (float)(curand(&randStates[blockIdx.x])); // Arvotaan optinen syvyys, jonka fotonipaketti kulkee.
		if (threadIdx.x == 0) printf("%f\n", tau);
		//int len = state->side_length;
		//while(1) {
		//	// Haetaan nykyisen paikan tiheys k‰ytt‰en intrinsikkist‰ Cuda-funktiota, jossa rd = round down
		//	float cur_density = cloud[__float2int_rd(cur_ray->position[0]) * len * len \
		//		+ __float2int_rd(cur_ray->position[1]) * len \
		//		+ __float2int_rd(cur_ray->position[2])];
		//	// Siirret‰‰n fotonipakettia yhden askeleen eteenp‰in
		//	cur_ray->position[0] += cur_ray->direction[0];
		//	cur_ray->position[1] += cur_ray->direction[1];
		//	cur_ray->position[2] += cur_ray->direction[2];
		//	tau -= state->kappa * cur_density * state->step_length; // V‰hennet‰‰n tau:sta kuljettu osa. Approksimoidaan tiheys vakioksi koko askeleelle.
		//	// Lopetetaan, jos on kuljettu tarvittava matka tai poistutaan simulaatioalueesta
		//	if (tau <= 0.0f) break;
		//	if (cur_ray->position[0] < 0.0f || cur_ray->position[0] > state->side_length_f) return RAY_SUCCESS;
		//	if (cur_ray->position[1] < 0.0f || cur_ray->position[1] > state->side_length_f) return RAY_SUCCESS;
		//	if (cur_ray->position[2] < 0.0f || cur_ray->position[2] > state->side_length_f) return RAY_SUCCESS;
		//}
		//scatter();
	}
	// MAX_SCATTER saavutettu
	return RAY_TRAPPED;
}

// Kukin s‰ie laskee MC-s‰teilynkuljetuksen yhdelle fotonipaketille
void trace_ray(float *cloud, float *map, int *dims, curandStateMtgp32 *randStates) {
	ray *cur_ray = init_ray_planar(dims, randStates);
	if (threadIdx.x == 0) {
		print_ray(cur_ray);
		printf("%f\n", cur_ray->intensity);
		printf("jee");
	}
	//int ray_status = simulate_ray(cloud, cur_ray, randStates);


	//process_ray();
	free(cur_ray);
}

__device__ void process_ray() {

}
// Alustaa vakiotiheyksisen pilven
float* generate_constant_density_cloud(int size) {
	size_t cloud_size = size*size*size*sizeof(float);
	float *cloud = (float *)malloc(cloud_size);
	for (int i = 0; i < size*size*size; i++) {
		cloud[i] = 1.0f;
	}
	return cloud;
}

int main(void) {
	// Simulaation parametreja
	int dim_size = 100; // Kunkin dimension suuruus
	int num_rays = 50; // L‰hetett‰vien fotonipakettien lkm.

	long randSeed = SEED;

	// T‰h‰n listaan eri dimensioiden koot
	int *dims = (int *)malloc(NUM_DIMS * sizeof(int));
	for (int i = 0; i < NUM_DIMS; i++) {
		dims[i] = dim_size;
	}

	// Tehd‰‰n pilvimalli ja siirret‰‰n se laitteelle
	float *hCloud = generate_constant_density_cloud(dim_size);
	float *dCloud;
	CUDA_CALL(cudaMalloc((void **)&dCloud, dim_size * dim_size * dim_size * sizeof(float)));
	CUDA_CALL(cudaMemcpy(dCloud, hCloud, dim_size * dim_size * dim_size * sizeof(float), cudaMemcpyHostToDevice));
	free(hCloud);

	// Pilven parametrit GPU:lle
	cloud_state *hcloud_state = (cloud_state *)malloc(sizeof(cloud_state));
	hcloud_state->albedo = 0.8f;
	hcloud_state->cell_side_length = 20.0f;
	hcloud_state->step_length = hcloud_state->cell_side_length * STEP;
	hcloud_state->kappa = 10.0f;
	hcloud_state->side_length = dim_size;
	hcloud_state->side_length_f = (float)dim_size;
	CUDA_CALL(cudaMemcpyToSymbol(state, (const void *)hcloud_state, sizeof(cloud_state)));

	// Varataan muistia ja alustetaan rng
	curandStateMtgp32 *dRandStates;
    mtgp32_kernel_params *dRandParams;
	CUDA_CALL(cudaMalloc((void **)&dRandStates, gridSize * sizeof(curandStateMtgp32))); // Jokainen lohko k‰ytt‰‰ omaa rng:n tilaa.
	CUDA_CALL(cudaMalloc((void **)&dRandParams, sizeof(mtgp32_kernel_params)));

	// K‰ynnistet‰‰n ydin
	trace_ray<<<gridSize, blockSize>>>(dCloud, NULL, ddims, dRandStates);

	CUDA_CALL(cudaDeviceSynchronize());
	// Poistetaan muistivarauksia laitteelta
	CUDA_CALL(cudaFree(dCloud));
	CUDA_CALL(cudaFree(ddims));
	CUDA_CALL(cudaFree(dRandStates));
	CUDA_CALL(cudaFree(dRandParams));
    return 0;
}
