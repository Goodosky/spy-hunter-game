#include "headers/spy_hunter.h"
#include <cstdlib>
#include <iostream>
using namespace std;


SpyHunter::SpyHunter() {
}

void SpyHunter::addRoadLevel() {
	// Get a random number in range 0-999
	int random = rand() % 1000;

	// and assign an action (possibility: 4/1000)
	if (random == 0 || random == 1) {
		// We change start
		int start_shift = random == 0 ? TURN_SIZE : -TURN_SIZE;
		int new_start = _road[SCREEN_HEIGHT - 1].start + start_shift;
		if (new_start < INITIAL_ROAD_START && new_start > TURN_SIZE) {
			_road[SCREEN_HEIGHT].start = _road[SCREEN_HEIGHT - 1].start + start_shift;
		}
	}

	if (random == 2 || random == 3) {
		// We change end
		int end_shift = random == 2 ? TURN_SIZE : -TURN_SIZE;
		int new_end = _road[SCREEN_HEIGHT - 1].end + end_shift;
		if (new_end > INITIAL_ROAD_END && new_end < SCREEN_WIDTH - TURN_SIZE) {
			_road[SCREEN_HEIGHT].end = _road[SCREEN_HEIGHT - 1].end + end_shift;
		}
	}
}

void SpyHunter::drawRoad() {
	for (int y = 0; y <= SCREEN_HEIGHT; y++) {
		// Draw road level
		//cout << _road[y].start << endl;
		int road_width = _road[y].end - _road[y].start;
		_sdl.drawRectangle(_road[y].start, y, road_width, 1, _sdl.colors.road);

		// set new values for the next frame
		if (y == SCREEN_HEIGHT) {
			// Set new road level
			addRoadLevel();
		}
		else {
			// Copy from next road level (last one fade out)
			_road[y].start = _road[y + 1].start;
			_road[y].end = _road[y + 1].end;
		}
	}
}

void SpyHunter::drawFrame() {
	_sdl.clearScreen();
	_sdl.updateTimeBasedValues();
	_delta = _sdl.getDelta();

	drawRoad();

	_sdl.drawLegend(_score);
	_sdl.refreshScreen();
}



void SpyHunter::endGame() {
	_sdl.freeAllSurfacesAndQuit();
}