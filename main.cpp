#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>

#include "headers/sdl_controller.h"
#include "headers/spy_hunter.h"

// main
#ifdef __cplusplus
extern "C"
#endif


int main(int argc, char** argv) {
	SDL_Event event;
	SdlController sdl;
	SpyHunter game;

	while (true) {
		//if (game.getIsPause) continue;

		sdl.clearScreen();
		sdl.updateTimeBasedValues();
		sdl.drawLegend();

		sdl.refreshScreen();

		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE) exit(0);
				//else if (event.key.keysym.sym == SDLK_UP) etiSpeed = 2.0;
				//else if (event.key.keysym.sym == SDLK_DOWN) etiSpeed = 0.3;
				break;
			case SDL_KEYUP:
				break;
			case SDL_QUIT:
				exit(0);
				break;
			};
		};
	};

	sdl.freeAllSurfacesAndQuit();

	return 0;
};
