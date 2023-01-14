#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>


#include "headers/spy_hunter.h"

// main
#ifdef __cplusplus
extern "C"
#endif


int main(int argc, char** argv) {
	bool quit = false;
	SDL_Event event;
	SpyHunter game;

	while (!quit) {
		//if (game.getIsPause) continue;

		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE) quit = true;
				break;
			case SDL_KEYUP:
				break;
			case SDL_QUIT:
				quit = true;
				break;
			};
		};

		game.drawFrame();
	};

	game.endGame();

	return 0;
};
