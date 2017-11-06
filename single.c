
/* malloc, free */
#include <stdlib.h>

/* for tests and possibly others. */
#include <assert.h>

/* cos, sin, tan, sqrt, etc */
#include <math.h>

/* header file */
#include "servers.h"

/* The amount an npc moves after every iteration */
#define KM_PER_IT (0.01f / 1000.0f)

/* move all of the npcs based on the global speed, their
 * direction and their coordinates. This function does NOT
 * move them back in bounds.
 */

#define PI 3.14159265

static void move_npcs(npc *npcs, uint64_t npcs_per_continent) {
	for (uint64_t i = 0; i < npcs_per_continent; ++i) {
		npcs[i].x += cos(npcs[i].dir * PI / 180.0) * KM_PER_IT;
		npcs[i].y += sin(npcs[i].dir * PI / 180.0) * KM_PER_IT;
	}
}

/* move all of the npcs that are out of bounds, in bounds. */
static void move_in_bounds(npc *npcs, uint64_t npcs_per_continent) {
	for (uint64_t i = 0; i < npcs_per_continent; ++i) {
		if (npcs[i].x > 1.0) npcs[i].x = 1.0;
		if (npcs[i].x < 0.0) npcs[i].x = 0.0;		
		if (npcs[i].y > 1.0) npcs[i].y = 1.0;
		if (npcs[i].y < 0.0) npcs[i].y = 0.0;
	}
}

/* Assign every npc a random x, y and direction. Set inter to 0 */
static void fill_continent(npc *npcs, uint64_t npcs_per_continent) {
	for (uint64_t i = 0; i < npcs_per_continent; ++i) {
		npcs[i].x = (float) rand() / (float) RAND_MAX;
		npcs[i].y = (float) rand() / (float) RAND_MAX;
		npcs[i].dir = (float) rand() / (float) RAND_MAX * 360.0f;
	}
}

#define DEC 1000

/* return a random float from 0.0 to 360.0 based on the given long ints. */
static inline float get_random_float_360(uint64_t one, uint64_t two) {
	uint64_t prod = one * two;
	return (double)(prod % (360 * DEC)) / DEC;
}

/* If one of the x or y values of the npc is 0.0 or 1.0, then the npc must get a new direction. */
static void give_new_dir(npc *npcs, uint64_t npcs_per_continent) {
	for (uint64_t i = 0; i < npcs_per_continent; ++i) {
		if (npcs[i].x == 1.0) npcs[i].dir = get_random_float_360(i, 321);
		if (npcs[i].x == 0.0) npcs[i].dir = get_random_float_360(i, 321);
		if (npcs[i].y == 1.0) npcs[i].dir = get_random_float_360(i, 321);
		if (npcs[i].y == 0.0) npcs[i].dir = get_random_float_360(i, 321);
	}
}

uint64_t start_single(uint64_t seed, uint16_t num_continents, uint64_t npcs_per_continent, float range_to_interact, uint64_t num_iterations) {
	/* initalize random seed */
	srand(seed);

	/* allocate the correct amount of npcs.
	 * first malloc is for each continent
	 * second is to allocate for all of the others
	 */
	npc **npcs = (npc**) malloc(num_continents * sizeof(npc*));
	for (uint16_t i = 0; i < num_continents; ++i)
		npcs[i] = (npc*) malloc(npcs_per_continent * sizeof(npc*));

	/* free allocated memory */
	for (uint16_t i = 0; i < num_continents; ++i)
		free(npcs[i]);
	free(npcs);
	
}

#if defined TEST_SINGLE_
/* This is an optional test section. compile with:
 * gcc single.c -g -D TEST_SINGLE_ -o testsingle -lm 
 * to run it
 */

#define QUIET 0
#define VERBOSE 1

/* for printing info */
#include <stdio.h>
/* for seed */
#include <time.h>

#define PREC 10000.0

uint8_t feq(float one, float two) {
	if((uint32_t)(one * PREC) == (uint32_t)(two * PREC)) {
		return 1;
	}
	return 0;
}

void print_npc(npc *pr) {
	printf("x: %f, y: %f, dir: %f\n", pr->x, pr->y, pr->dir);
}

/* check if all of the npcs in a list have legal values */
void test_correct_npc(uint64_t len, npc *npcs, uint8_t vol) {
	for (uint64_t i = 0; i < len; ++i) {
		if (vol == VERBOSE)
			print_npc(&npcs[i]);
		assert(npcs[i].x >= 0.0);
		assert(npcs[i].y >= 0.0);
		assert(npcs[i].x <= 1.0);
		assert(npcs[i].y <= 1.0);
		assert(npcs[i].dir >= 0.0);
		assert(npcs[i].dir <= 360.0);
		assert(npcs[i].inter == 0);
	}
}

void test_fill_continent() {
	printf("Testing filling continents.\n");
	/* test a small continent */
	uint64_t len = 16;
	npc *npcs = (npc*) malloc(len * sizeof(npc));
	fill_continent(npcs, len);
	test_correct_npc(len, npcs, VERBOSE);
	free(npcs);
	/* test a very large amount */
	len = 1024 * 1024;
	npcs = (npc*) malloc(len * sizeof(npc));
	fill_continent(npcs, len);
	test_correct_npc(len, npcs, QUIET);
	free(npcs);
	printf("Done testing filling continents.\n");
}


/* this test only works when KM_PER_IT is 0.00001 */
void test_move_npcs() {
	printf("\nBeginning test on moving npcs.\n");
	/* a list of six NPCs we will do operations on */
	uint64_t n = 6;
	npc npcs[n];
	npcs[0].x = 0.1f; npcs[0].y = 0.2f; npcs[0].dir = 25.0f;
	npcs[1].x = 0.3f; npcs[1].y = 0.7f; npcs[1].dir = 36.0f;
	npcs[2].x = 0.6f; npcs[2].y = 0.4f; npcs[2].dir = 0.0f;
	npcs[3].x = 0.1f; npcs[3].y = 0.3f; npcs[3].dir = 150.0f;
	npcs[4].x = 0.52f; npcs[4].y = 0.2f; npcs[4].dir = 190.0f;
	npcs[5].x = 0.105f; npcs[5].y = 0.9f; npcs[5].dir = 360.0f;

	move_npcs(npcs, n);

	/* x and y results */
	float res[n * 2];
	uint8_t i = 0;
	res[i++] = 0.10000906; res[i++] = 0.20000422;	
	res[i++] = 0.30000809; res[i++] = 0.70000588;
	res[i++] = 0.60001; res[i++] = 0.4;
	res[i++] = 0.09999133; res[i++] = 0.300005;
	res[i++] = 0.51999015; res[i++] = 0.1999982;
	res[i++] = 0.10501; res[i++] = 0.9;

	for (uint8_t i1 = 0, i2 = 0; i1 < 6; ++i1) {
		printf("resx: %.8f npcx: %.8f\tresy %.8f npcy: %.8f\n", res[i2], npcs[i1].x, res[i2 + 1], npcs[i1].y);
		assert(feq(npcs[i1].x, res[i2++]));
		assert(feq(npcs[i1].y, res[i2++]));
	}
	printf("Done testing moving NPCs.\n");
}

void test_move_in_bounds() {
	printf("\nTesting move back into bounds\n");
	uint64_t n = 13;
	npc npcs[n];
	/* only x is negative */
	npcs[0].x = -0.001; npcs[0].y = 0.5;
	npcs[1].x = -654645.3; npcs[1].y = 1.0;
	/* only y is negative */
	npcs[2].x = 0.3; npcs[2].y = -0.3;
	npcs[3].x = 0.9; npcs[3].y = -0.99999;
	/* both are negative */	
	npcs[4].x = -0.8; npcs[4].y = -0.3;
	npcs[5].x = -0.01; npcs[5].y = -0.1;
	/* only x is over 1.0 */
	npcs[6].x = 1.00001; npcs[6].y = 0.3;
	npcs[7].x = 3.000; npcs[7].y = 0.999;
	/* only y is over 1.0 */
	npcs[8].x = 0.000; npcs[8].y = 1.00001;
	npcs[9].x = 0.999; npcs[9].y = 4.0000;
	/* both above 1.0 */
	npcs[10].x = 1.0001; npcs[10].y = 1.0001;
	/* x neg y over 1.0 */
	npcs[11].x = -0.0001; npcs[11].y = 1.0001;
	/* x over 1.0 y neg */
	npcs[12].x = 1.001; npcs[12].y = -0.001;

	move_in_bounds(npcs, n);

	/* feq is needed because of floating point errors. */
	assert(npcs[0].x == 0.0);
	assert(npcs[0].y == 0.5);
	assert(npcs[1].x == 0.0);
	assert(npcs[1].y == 1.0);
	assert(feq(npcs[2].x, 0.3));
	assert(npcs[2].y == 0.0);
	assert(feq(npcs[3].x, 0.9));
	assert(npcs[3].y == 0.0);
	assert(npcs[4].x == 0.0);
	assert(npcs[4].y == 0.0);
	assert(npcs[5].x == 0.0);
	assert(npcs[5].y == 0.0);
	assert(npcs[6].x == 1.0);
	assert(feq(npcs[6].y, 0.3));
	assert(npcs[7].x == 1.0);
	assert(feq(npcs[7].y, 0.999));
	assert(npcs[8].x == 0.000);
	assert(npcs[8].y == 1.0);
	assert(feq(npcs[9].x, 0.999));
	assert(npcs[9].y == 1.0);
	assert(npcs[10].x == 1.0);
	assert(npcs[10].y == 1.0);
	assert(npcs[11].x == 0.0);
	assert(npcs[11].y == 1.0);
	assert(npcs[12].x == 1.0);
	assert(npcs[12].y == 0.0);	
	printf("Done testing move into bounds.\n");
}

void test_get_random_float_360() {
	printf("\nStarting float 360 tests\n");
	uint64_t n = 8;
	uint64_t num = 32123;
	float ra;
	for (uint64_t i = 0; i < n; ++i) {
		ra = get_random_float_360(i, num);
		printf("Random float for %lu and %lu is: %f\n", i, num, ra);
		assert(ra <= 360.0 && ra >= 0.0);
	}

	printf("\nFloat 360 tests are completed.\n");
}

void test_give_new_dir() {
	printf("\nTest giving new directions.\n");
	uint64_t n = 5;
	npc npcs[n];
	npcs[0].x = 0.0; npcs[0].y = 0.5; npcs[0].dir = 8.125;
	npcs[1].x = 0.6; npcs[1].y = 1.0; npcs[1].dir = 32.0;
	npcs[2].x = 1.0; npcs[2].y = 0.0; npcs[2].dir = 256.0;
	npcs[3].x = 0.0; npcs[3].y = 1.0; npcs[3].dir = 128.0;
	npcs[4].x = 0.4; npcs[4].y = 0.1; npcs[4].dir = 256.0;

	give_new_dir(npcs, n);

	printf("The new direction of npcs at %d is %f\n", 0, npcs[0].dir);
	assert(npcs[0].dir != 8.125);
	printf("The new direction of npcs at %d is %f\n", 1, npcs[1].dir);
	assert(npcs[1].dir != 32.0);
	printf("The new direction of npcs at %d is %f\n", 2, npcs[2].dir);
	assert(npcs[2].dir != 256.0);
	printf("The new direction of npcs at %d is %f\n", 3, npcs[3].dir);
	assert(npcs[3].dir != 128.0);
	printf("This value should not change.\n");
	assert(npcs[4].dir == 256.0);

	printf("\nTest giving new directions completed.\n");
}

int main() {
	srand(time(NULL));
	test_fill_continent();
	test_move_npcs();
	test_move_in_bounds();
	test_get_random_float_360();
	test_give_new_dir();
}

#endif
