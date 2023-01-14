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


void SpyHunter::addRoadLevel() {
	// Get a random number in range from 0 to ROAD_TURN_POSSIBILITY-1
	int random = rand() % ROAD_TURN_POSSIBILITY;

	// and assign an action (possibility: 4/TURN_POSSIBILITY-1)
	if (random == 0 || random == 1) {
		// We change start
		int start_shift = random == 0 ? ROAD_TURN_SIZE : -ROAD_TURN_SIZE;
		int new_start = _road[SCREEN_HEIGHT - 1].start + start_shift;
		if (new_start < INITIAL_ROAD_START && new_start > ROAD_TURN_SIZE) {
			_road[SCREEN_HEIGHT].start = _road[SCREEN_HEIGHT - 1].start + start_shift;
		}
	}

	if (random == 2 || random == 3) {
		// We change end
		int end_shift = random == 2 ? ROAD_TURN_SIZE : -ROAD_TURN_SIZE;
		int new_end = _road[SCREEN_HEIGHT - 1].end + end_shift;
		if (new_end > INITIAL_ROAD_END && new_end < SCREEN_WIDTH - ROAD_TURN_SIZE) {
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

void SpyHunter::drawFrame(bool is_resuming) {
	_sdl.clearScreen();
	_sdl.updateTimeBasedValues(is_resuming);
	_delta = _sdl.getDelta();
	_score += _delta;

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