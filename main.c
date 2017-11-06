#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>


uint8_t get_help(int argc, char **argv) {
	for (int i = 0; i < argc; ++i) {
		if (strcmp(argv[i], "-h") == 0) {
			return 1;
		}
	}
	return 0;
}

uint8_t get_seed(uint64_t *seed, int argc, char **argv) {
	for (int i = 0; i < argc - 1; ++i) {
		if (strcmp(argv[i], "-s") == 0) {
			*seed = atoi(argv[i + 1]);
			return 0;
		}
	}
	if (strcmp(argv[argc - 1], "-s") == 0) {
		return 1;
	}
	*seed = time(NULL);
	return 0;
}

#define DEF_NUM_CONT 16

uint8_t get_num_continents(uint16_t *num_continents, int argc, char **argv) {
	for (int i = 0; i < argc - 1; ++i) {
		if (strcmp(argv[i], "-nc") == 0) {
			*num_continents = atoi(argv[i + 1]);
			return 0;
		}
	}
	if (strcmp(argv[argc - 1], "-s") == 0) {
		return 1;
	}
	*num_continents = DEF_NUM_CONT;
	return 0;
}

#define DEF_NPC_PER (1024)

uint8_t get_npcs_per_continent(uint64_t *npcs_per_continent, int argc, char **argv) {
	for (int i = 0; i < argc - 1; ++i) {
		if (strcmp(argv[i], "-npcpc") == 0) {
			*npcs_per_continent = atoi(argv[i + 1]);
			return 0;
		}
	}
	if (strcmp(argv[argc - 1], "-npcpc") == 0) {
		return 1;
	}
	*npcs_per_continent = DEF_NPC_PER;
	return 0;
}

#define DEF_RANGE_TO 0.0125f

uint8_t get_range_to_interact(float *range_to_interact, int argc, char **argv) {
	for (int i = 0; i < argc - 1; ++i) {
		if (strcmp(argv[i], "-rti") == 0) {
			*range_to_interact = atof(argv[i + 1]);
			return 0;
		}
	}
	if (strcmp(argv[argc - 1], "-rti") == 0) {
		return 1;
	}
	*range_to_interact = DEF_RANGE_TO;
	return 0;
}

#define DEF_NUM_IT (1024)

uint8_t get_num_iterations(uint64_t *num_iterations, int argc, char **argv) {
	for (int i = 0; i < argc - 1; ++i) {
		if (strcmp(argv[i], "-ni") == 0) {
			*num_iterations = atoi(argv[i + 1]);
			return 0;
		}
	}
	if (strcmp(argv[argc - 1], "-ni") == 0) {
		return 1;
	}
	*num_iterations = DEF_NUM_IT;
	return 0;
}


enum device_ids{single=0, multi, gpu=2};

#define DEF_DEV (single)

uint8_t get_device(uint8_t *device, int argc, char **argv) {
	for (int i = 0; i < argc - 1; ++i) {
		if (strcmp(argv[i], "-dev") == 0) {
			*device = atoi(argv[i + 1]);
		}
	}
	if (strcmp(argv[argc - 1], "-dev") == 0 || *device > gpu) {
		return 1;
	}
	else
		return 0;
	*device = DEF_DEV;
	return 0;
}
/* the seed of generation */
uint64_t seed;
/* the amount of continents */
uint16_t num_continents;
/* the fixed amount of npcs per continent */
uint64_t npcs_per_continent;
/* how close the npcs have to be together to interact */
float range_to_interact;
/* how many iterations the program should run */
uint64_t num_iterations;
/* what device we will run on */
uint8_t device;

int main(int argc, char **argv) {
	/* see if the user needs help */
	if (get_help(argc, argv)) {
		printf("Help\n \
			Follow commands by a space, then the value.\n \
			Not specifying a value will lead to a default.\n \
			-s\tseed\n \
			-nc\tnumber of continents\n \
			-npcpc\tnpcs per continent\n \
			-rti\trange to interact\n \
			-ni\tnumber of iterations\n \
			-dev\tthe device (0 = 1 thread, 1 = multi, 2 = GPU)\n");
		return 1;
	}
	else {
		printf("Type -h for help.\n");
	}
	/* Process input through argv */
	if (get_seed(&seed, argc, argv)) {
		printf("Error parsing seed. Exiting.\n");
		return 1;
	}
	printf("Seed: %lu\n", seed);
	if (get_num_continents(&num_continents, argc, argv)) {
		printf("Error parsing number of continents. Exiting.\n");
		return 1;
	}
	printf("Number of continents: %d\n", num_continents);

	if (get_npcs_per_continent(&npcs_per_continent, argc, argv)) {
		printf("Error parsing number of npcs per continent. Exiting.\n");
		return 1;
	}

	printf("npcs per continent: %lu\n", npcs_per_continent);

	if (get_range_to_interact(&range_to_interact, argc, argv)) {
		printf("Error parsing range to interact. Exiting.\n");
		return 1;
	}
	printf("range to interact: %f\n", range_to_interact);

	if (get_num_iterations(&num_iterations, argc, argv)) {
		printf("Error parsing number iterations. Exiting.\n");
		return 1;
	}
	printf("number of iterations: %lu\n", num_iterations);
	if (get_device(&device, argc, argv)) {
		printf("Error finding the device specified. Exiting.\n");
		return 1;
	}
	printf("device: %d\n", device);
	/* initilize game server
	switch (device) {
		case single:
			single_run(seed, num_continents, npcs_per_continent, range_to_interact, num_iterations);
			break;
	}*/
	/* get stats from game server */

	/* close game server */
	return EXIT_SUCCESS;
}
