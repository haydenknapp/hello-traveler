#include "contdisplay.h"

void display_cont_init(display_cont* this_display, uint16_t pixels_per_length, npc* npcs, uint64_t n_npcs) {
	/* general and npc info */
	this_display->n_npcs = n_npcs;
	this_display->pixels_per_length = pixels_per_length;
	this_display->npcs = npcs;
	this_display->stop = 0;
	/* SDL stuff */
	SDL_Init(SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer(pixels_per_length, pixels_per_length, 0, &this_display->window, &this_display->renderer);
}


void display_cont_destroy(display_cont* this_display) {
	SDL_DestroyWindow(this_display->window);
	SDL_DestroyRenderer(this_display->renderer);
	SDL_Quit();
}


void display_cont_update(display_cont* this_display) {
	SDL_Event event;
	SDL_SetRenderDrawColor(this_display->renderer, 0, 0, 0, 0);
	SDL_RenderClear(this_display->renderer);
	SDL_SetRenderDrawColor(this_display->renderer, 255, 255, 255, 255);
	for (uint64_t i = 0; i < this_display->n_npcs; ++i) {
		SDL_RenderDrawPoint(
				this_display->renderer,
				(uint32_t)(this_display->npcs[i].x * this_display->pixels_per_length),
				(uint32_t)(this_display->npcs[i].y * this_display->pixels_per_length));
	}
	if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
		this_display->stop = 1;
	SDL_RenderPresent(this_display->renderer);
}
