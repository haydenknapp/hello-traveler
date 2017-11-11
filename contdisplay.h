#ifndef DISPLAY_CONT_H_
#define DISPLAY_CONT_H_

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h>

#include "servers.h"

/* This is a way for the viewer to view npc movements. It only
 * works on one continent and should only be used for debugging
 * or a proof of npc movement, as it slows the program down.
 */

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
