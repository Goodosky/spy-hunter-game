#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>


#include "headers/config.h"
#include "headers/spy_hunter.h"

// main
#ifdef __cplusplus
extern "C"
#endif


int main(int argc, char** argv) {
	bool quit, is_pause, is_resuming;
	SDL_Event event;
	SpyHunter game;

	quit = is_pause = is_resuming = false;

	while (!quit) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_p:
					is_pause = !is_pause;
					is_resuming = true;
					break;
				case SDLK_RIGHT:
					game.turn(right);
					break;
				case SDLK_LEFT:
					game.turn(left);
					break;
				case SDLK_ESCAPE:
					quit = true;
					break;
				}
				break;
			case SDL_KEYUP:
				if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_LEFT)
					game.turn(off);
				break;
			case SDL_QUIT:
				quit = true;
				break;
			};
		};

		if (!is_pause) game.drawFrame(is_resuming);
		is_resuming = false;
	};

	game.endGame();

	return 0;
};
