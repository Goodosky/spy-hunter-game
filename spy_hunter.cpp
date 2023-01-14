#include "headers/spy_hunter.h"
#include <cstdlib>
#include <ctime> 
#include <iostream>
using namespace std;

SpyHunter::SpyHunter() {
	srand(time(NULL)); // current time as seed for random generator
	startGame();
}

void SpyHunter::startGame() {
	// Set default values
	_score = 0;
	_elapsedMapUpdate = MAP_REFRESH_INTERVAL;

	for (int y = 0; y <= SCREEN_HEIGHT; y++) {
		_road[y].start = INITIAL_ROAD_START;
		_road[y].end = INITIAL_ROAD_END;
	}

	// Create player
	_player.x = SCREEN_WIDTH / 2 - (INITIAL_ROAD_END - INITIAL_ROAD_START) / 2; // (in center)
	_player.y = SCREEN_HEIGHT - 100; // 100px from bottom
	_player.surface = _sdl.loadBmp("./media/car.bmp");
}


void SpyHunter::drawPlayer() {
	if (_turning == Turning::right) {
		_player.x += TURN_SPEED * _delta;
	}
	else if (_turning == Turning::left) {
		_player.x -= TURN_SPEED * _delta;
	}

	drawSprite(_player);
}

void SpyHunter::turn(Turning direction) {
	_turning = direction;
}

void SpyHunter::drawRoad() {
	for (int y = SCREEN_HEIGHT; y >= 0; y--) {
		int road_width = _road[y].end - _road[y].start;
		_sdl.drawRectangle(_road[y].start, y, road_width, 1, _sdl.colors.road);
	}
}

void SpyHunter::addRoadLevel() {
	int start = _road[0].start;
	int end = _road[0].end;

	// Draw randomly change the value of the start and end of the road:
	// 1. Get a random number in range from 0 to ROAD_TURN_PROBABILITY-1
	int random = rand() % ROAD_TURN_PROBABILITY;

	// Match change (propbability to change for each direction: 2/ROAD_TURN_PROBABILITY-1)
	if (random == 0 || random == 1) {
		// We are changing start
		int start_shift = random == 0 ? ROAD_TURN_SIZE : -ROAD_TURN_SIZE;
		if (_road[0].start + start_shift < INITIAL_ROAD_START && _road[0].start + start_shift > ROAD_TURN_SIZE) {
			start = _road[0].start + start_shift;
		}
	}

	else if (random == 2 || random == 3) {
		// We are changing end
		int end_shift = random == 2 ? ROAD_TURN_SIZE : -ROAD_TURN_SIZE;
		if (_road[0].end + end_shift > INITIAL_ROAD_END && _road[0].end + end_shift < SCREEN_WIDTH - ROAD_TURN_SIZE) {
			end = _road[0].end + end_shift;
		}
	}

	// Add new values to _road
	for (int y = 0; y <= ROAD_LEVEL_HEIGHT; y++) {
		_road[y].start = start;
		_road[y].end = end;
	}
}

void SpyHunter::updateRoad() {
	// set new _road values for the next frame
	for (int y = SCREEN_HEIGHT; y >= 0; y--) {
		if (y <= ROAD_LEVEL_HEIGHT) {
			addRoadLevel();
		}
		else {
			// Copy from next road level (last one fade out)
			_road[y].start = _road[y - ROAD_LEVEL_HEIGHT].start;
			_road[y].end = _road[y - ROAD_LEVEL_HEIGHT].end;
		}
	}
}


void SpyHunter::drawFrame(bool is_resuming) {
	_sdl.updateDelta(is_resuming);
	_delta = _sdl.getDelta(); // delta in seconds
	_elapsedMapUpdate += _delta;

	_sdl.clearScreen();
	_sdl.updateDeltaBasedValues();
	_score += _delta;

	if (_elapsedMapUpdate >= MAP_REFRESH_INTERVAL) {
		updateRoad();
		_elapsedMapUpdate = 0;
	}

	drawRoad();
	drawPlayer();

	_sdl.drawLegend(_score);
	_sdl.refreshScreen();
}

void SpyHunter::drawSprite(Sprite sprite) {
	_sdl.drawSprite(sprite.surface, sprite.x, sprite.y);
}


void SpyHunter::endGame() {
	_sdl.freeAllSurfacesAndQuit();
}