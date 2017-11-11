/* Hayden Knapp
 *
 * Header file for the continent display. The continent display is useful for seeing how
 * npcs move in only one continent. This helps insure that the npcs are moving properly
 * and serves as a visual test. This feature slows the program down however, and is an
 * argument for the hello traveler program.
 */

#ifndef DISPLAY_CONT_H_
#define DISPLAY_CONT_H_

/* standard ints for args */
#include <stdint.h>
/* SDL2 libraries for drawing */
#include <SDL2/SDL.h>
/* used only to get the npc structure */
#include "servers.h"

/* Store information related to a continent display.
 * This can consist of NPC and continent info used
 * to communicate between the server and this drawer,
 * or it can hold SDL resources.
 */
typedef struct {
	/* general */
	uint16_t pixels_per_length;
	uint8_t stop;
	/* npc info. */
	npc* npcs;
	uint64_t n_npcs;

	/* SDL resources */
	SDL_Window *window;
	SDL_Renderer *renderer;
} display_cont;


/* Initialize the continent display. This function
 * takes a pointer to a display, the amount of pixels
 * per length of continent, the list of npcs an the
 * number of npcs.
 */
void display_cont_init(display_cont*, uint16_t, npc*, uint64_t);

/* Delete any resources that this display has allocated. */
void display_cont_destroy(display_cont*);

/* Update what the window is showing based on
 * the provided display's information has
 */
void display_cont_update(display_cont*);

#endif
