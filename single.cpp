/* Hayden Knapp
 *
 * Implementation of the single threaded npc processor found in servers.h
 */


/* malloc, free */
#include <stdlib.h>
/* for tests and possibly others. */
#include <assert.h>
/* cos, sin, tan, sqrt, etc */
#include <math.h>
/* header file */
#include "servers.h"
/* for the display */
#include "contdisplay.h"

/* print an npc */
static void print_npc(npc *pr) {
	printf("x: %f, y: %f, dir: %f\n", pr->x, pr->y, pr->dir);
}

/* move all of the npcs based on the global speed, their
 * direction and their coordinates. This function does NOT
 * move them back in bounds.
 */

#define PI 3.14159265

static void move_npcs(npc *npcs, uint64_t npcs_per_continent, float npc_speed) {
	for (uint64_t i = 0; i < npcs_per_continent; ++i) {
		npcs[i].x += cos(npcs[i].dir * PI / 180.0) * npc_speed;
		npcs[i].y += sin(npcs[i].dir * PI / 180.0) * npc_speed;
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
		npcs[i].inter = 0;
	}
	/* break here */
}

#define DEC 1000

/* If one of the x or y values of the npc is 0.0 or 1.0, then the npc must get a new direction. */
static void give_new_dir(npc *npcs, uint64_t npcs_per_continent, uint64_t extra) {
	for (uint64_t i = 0; i < npcs_per_continent; ++i) {
		if (npcs[i].x > 0.9999) npcs[i].dir += 3.5 * extra;
		if (npcs[i].x < 0.0001) npcs[i].dir += 3.5 * extra;
		if (npcs[i].y > 0.9999) npcs[i].dir += 3.5 * extra;
		if (npcs[i].y < 0.0001) npcs[i].dir += 3.5 * extra;
	}
}

/* This function has every npc interact with another. */
static void interact_with_all(npc *npcs, uint64_t npcs_per_continent, float range_to_interact) {
	/* curid is the index of the npcs giving interactions */
	float xdist, ydist, dist;
	for (uint64_t curid = 0; curid < npcs_per_continent; ++curid) {
		/* recid is for the npc that RECieves the interaction (is incremented) */
		for (uint64_t recid = 0; recid < npcs_per_continent; ++recid) {
			xdist = npcs[recid].x - npcs[curid].x;
			ydist = npcs[recid].y - npcs[curid].y;
			dist = sqrt(xdist * xdist + ydist * ydist);
			if (dist < range_to_interact && curid != recid)
				++npcs[recid].inter;
		}
	}
}

/* returns the count of how many interactions there were */
static uint64_t get_total_count(npc *npcs, uint64_t npcs_per_continent) {
	uint64_t ret = 0;
	for (uint64_t i = 0; i < npcs_per_continent; ++i)
		ret += npcs[i].inter;
	return ret;
}

#ifndef TEST_SINGLE_

uint64_t start_single(uint64_t seed, uint16_t num_continents, uint64_t npcs_per_continent, float range_to_interact, uint64_t num_iterations, float npc_speed, uint8_t disp) {
	/* initalize random seed */
	srand(seed);

	/* allocate the correct amount of npcs.
	 * first malloc is for each continent
	 * second is to allocate for all of the others
	 */
	npc **npcs = (npc**) malloc(num_continents * sizeof(npc*));
	for (uint16_t i = 0; i < num_continents; ++i) { 
		npcs[i] = (npc*) malloc(npcs_per_continent * sizeof(npc));
		fill_continent(npcs[i], npcs_per_continent);
	}

	
	display_cont display;
	/* Initialize the renderer */
	if (disp) {
		display_cont_init(&display, 1000, npcs[0], npcs_per_continent);
	}

	/* outer loop controls the iterations given. */
	for (uint64_t it = 0; it < num_iterations; ++it) {
		/* This loop controls every continent */
		for (uint16_t cont = 0; cont < num_continents; ++cont) {
			/* move all of the npcs in this continent. */
			move_npcs(npcs[cont], npcs_per_continent, npc_speed);
			/* move back in bounds if need be */
			move_in_bounds(npcs[cont], npcs_per_continent);
			/* give them new directions if need be */
			give_new_dir(npcs[cont], npcs_per_continent, it);
			/* check if they interacted */
			interact_with_all(npcs[cont], npcs_per_continent, range_to_interact);
		}
		/* check if the display needs to stop */
		if (disp) {
			display_cont_update(&display);
	//		print_npc(&display.npcs[0]);
			if (display.stop)
				break;
		}
	}

	/* destory cont drawer */
	if (disp)
		display_cont_destroy(&display);

	/* free allocated memory and get total count */
	uint64_t ret = 0;
	for (uint16_t i = 0; i < num_continents; ++i) {
		ret += get_total_count(npcs[i], npcs_per_continent); 
		free(npcs[i]);
	}
	free(npcs);

	/* return the count */
	return ret;
}

#elif defined TEST_SINGLE_
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


/* Test if npcs move the correct amount. */
void test_move_npcs() {
	printf("\nBeginning test on moving npcs.\n");
	/* a list of six NPCs we will do operations on */
	uint64_t n = 6;
	npc npcs[n];
	/* The amount the npcs will move in an iteration */
	float ddistance = (0.01f / 1000.0f);
	npcs[0].x = 0.1f; npcs[0].y = 0.2f; npcs[0].dir = 25.0f;
	npcs[1].x = 0.3f; npcs[1].y = 0.7f; npcs[1].dir = 36.0f;
	npcs[2].x = 0.6f; npcs[2].y = 0.4f; npcs[2].dir = 0.0f;
	npcs[3].x = 0.1f; npcs[3].y = 0.3f; npcs[3].dir = 150.0f;
	npcs[4].x = 0.52f; npcs[4].y = 0.2f; npcs[4].dir = 190.0f;
	npcs[5].x = 0.105f; npcs[5].y = 0.9f; npcs[5].dir = 360.0f;

	move_npcs(npcs, n, ddistance);

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

	give_new_dir(npcs, n, 321);

	printf("The new direction of npcs at %d is %f\n", 0, npcs[0].dir);
//	assert(npcs[0].dir != 8.125);
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

void test_interact_with_all() {
	printf("\nTesting interact with all.\n");
	uint64_t n = 5;
	npc npcs[n];

	/* the current */
	uint8_t i = 0;
	npcs[i].x = 0.1; npcs[i].y = 0.1; npcs[i++].inter = 0;
	npcs[i].x = 0.1; npcs[i].y = 0.100001; npcs[i++].inter = 0;
	npcs[i].x = 0.100001; npcs[i].y = 0.1; npcs[i++].inter = 0;
	npcs[i].x = 0.2; npcs[i].y = 0.2; npcs[i++].inter = 0;
	npcs[i].x = 0.2001; npcs[i].y = 0.2; npcs[i++].inter = 0;
	
	i = 0;

	interact_with_all(npcs, n, 0.001f);

	printf("Npc[%d]: %lu\n", i, npcs[i].inter);
	assert(npcs[i++].inter == 2);
	printf("Npc[%d]: %lu\n", i, npcs[i].inter);
	assert(npcs[i++].inter == 2);
	printf("Npc[%d]: %lu\n", i, npcs[i].inter);
	assert(npcs[i++].inter == 2);
	printf("Npc[%d]: %lu\n", i, npcs[i].inter);
	assert(npcs[i++].inter == 1);
	printf("Npc[%d]: %lu\n", i, npcs[i].inter);
	assert(npcs[i++].inter == 1);

	/* test get total */
	assert(get_total_count(npcs, n) == 8);

	printf("\nDone testing interact with all.\n");
}

int main() {
	srand(time(NULL));
	test_fill_continent();
	test_move_npcs();
	test_move_in_bounds();
//	test_get_random_float_360();
	test_give_new_dir();
	test_interact_with_all();
}

#endif
