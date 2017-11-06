#ifndef SERVERS_H_
#define SERVERS_H_

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
	       		float range_to_interact, uint64_t num_iterations);

#endif
