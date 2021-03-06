/* Hayden Knapp
 *
 * Here are the prototypes for the three main functions that compute what npcs are
 * doing and when they interact. They all do the same and return the same, but
 * differ in what hardware processes them.
 */

#ifndef SERVERS_H_
#define SERVERS_H_

/* standard ints */
#include <stdint.h>


/* This is how all npcs will be kept track of. x and y are always between
 * 0.0 and 1.0 inclusive. direction should be between 0.0 and 360.0. The
 * inter value is how many interactions it has received.
 */
typedef struct {
	/* x and y coordinates of npc */
	float x, y;
	/* number of times an npc said hello to this one */
	uint64_t inter;
	/* the direction the npc is moving */
	float dir;
} npc;

uint64_t start_single(uint64_t seed, uint16_t num_continents, uint64_t npcs_per_continent,
	       		float range_to_interact, uint64_t num_iterations, float npc_speed, uint8_t display);

uint64_t start_multi(uint64_t seed, uint16_t num_continents, uint64_t npcs_per_continent,
	       		float range_to_interact, uint64_t num_iterations, float npc_speed, uint8_t num_threads);

uint64_t start_gpu(uint64_t seed, uint16_t num_continents, uint64_t npcs_per_continent,
	       		float range_to_interact, uint64_t num_iterations, float npc_speed, uint8_t display);

#endif
