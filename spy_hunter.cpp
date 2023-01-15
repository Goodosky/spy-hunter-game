#include "headers/spy_hunter.h"
#include <cstdlib>
#include <ctime> 

SpyHunter::SpyHunter() {
	srand(time(NULL)); // current time as seed for random generator
	startGame();
}

void SpyHunter::startGame() {
	// Set default values
	_score = _worldTime = 0;
	_elapsedMapUpdate = MAP_REFRESH_INTERVAL;
	_score_paused = false;
	_turning = off;

	for (int y = 0; y < SCREEN_HEIGHT; y++) {
		_road[y].start = INITIAL_ROAD_START;
		_road[y].end = INITIAL_ROAD_END;
	}

	// Create player
	_player.x = SCREEN_WIDTH / 2; // (in center)
	_player.y = SCREEN_HEIGHT - 100; // 100px from bottom
	_player.surface = _sdl.loadBmp("./media/car.bmp");
	_player.lives = 1;

	_sdl.updateDelta(true);
}

void SpyHunter::restartGame() {
	int scorebord_skip = 0;
	SDL_Event event;

	// Draw menu bg
	_sdl.drawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, _sdl.colors.legend_bg);

	// Draw menu texts
	char txt[SCREEN_WIDTH];
	int x = LEGEND_OFFSET_X + LEGEND_PADDING;
	int y = LEGEND_OFFSET_Y + LEGEND_PADDING;

	_sdl.drawString(x, y, "KONIEC!");

	sprintf(txt, "Twoj wynik to: %.lf", _score);
	y += 3 * 8; _sdl.drawString(x, y, txt);

	y += 4 * 8; _sdl.drawString(x, y, "Wybierz akcje:");

	y += 2 * 8; _sdl.drawString(x, y, "n - rozpocznij nowa gre");
	y += 2 * 8; _sdl.drawString(x, y, "s - zapisz wynik do tablicy wynikow");
	y += 2 * 8; _sdl.drawString(x, y, "esc - wyjdz z gry");

	y += 5 * 8; drawScoreboard(x, y);

	_sdl.refreshScreen();

	// Handle menu events
	while (true) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_s: saveToScoreboard(); drawScoreboard(x, y, scorebord_skip); break;
				case SDLK_n: startGame(); return; break;
				case SDLK_ESCAPE: exit(0); break;
				case SDLK_p: drawScoreboard(x, y, scorebord_skip, by_points); break;
				case SDLK_t: drawScoreboard(x, y, scorebord_skip, by_time); break;
				case SDLK_UP:
					if (scorebord_skip > 0) scorebord_skip--;
					drawScoreboard(x, y, scorebord_skip);
					break;
				case SDLK_DOWN:
					scorebord_skip++;
					drawScoreboard(x, y, scorebord_skip);
					break;
				} break;
			};
		};

		_sdl.refreshScreen();
	}
}

void SpyHunter::endGame() {
	_sdl.freeAllSurfacesAndQuit();
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

void SpyHunter::handleCollisions() {
	// ROAD
	int distance_from_left_edge = _player.x - _player.surface->w / 2 - _road[(int)_player.y].start;
	int distance_from_right_edge = _road[(int)_player.y].end - _player.x - _player.surface->w / 2;

	if (distance_from_left_edge < -ROADSIDE_WIDTH || distance_from_right_edge < -ROADSIDE_WIDTH) {
		// the player is off the road
		_player.lives--;
	}
	else if (distance_from_left_edge < 0 || distance_from_right_edge < 0) {
		// the player is on the roadside
		_score_paused = true;
	}
	else _score_paused = false;
}


void SpyHunter::drawRoad() {
	for (int y = SCREEN_HEIGHT - 1; y >= 0; y--) {
		int road_width = _road[y].end - _road[y].start;

		// Draw roadside
		_sdl.drawRectangle(_road[y].start - ROADSIDE_WIDTH, y, road_width + 2 * ROADSIDE_WIDTH, 1, _sdl.colors.roadside);

		// Draw asphalt
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
	for (int y = SCREEN_HEIGHT - 1; y >= 0; y--) {
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


void SpyHunter::drawFrame(bool is_resuming = false) {
	if (_player.lives == 0) {
		restartGame();
		return;
	}

	_sdl.updateDelta(is_resuming);
	_delta = _sdl.getDelta(); // delta in seconds
	_elapsedMapUpdate += _delta;
	_worldTime += _delta;

	_sdl.clearScreen();
	if (!_score_paused) _score += _delta;

	if (_elapsedMapUpdate >= MAP_REFRESH_INTERVAL) {
		updateRoad();
		_elapsedMapUpdate = 0;
	}

	handleCollisions();

	drawRoad();
	drawPlayer();

	_sdl.drawLegend(_score, _worldTime);
	_sdl.refreshScreen();
}

void SpyHunter::drawSprite(Sprite sprite) {
	_sdl.drawSprite(sprite.surface, sprite.x, sprite.y);
}


void SpyHunter::drawScoreboard(int x, int y, int skip, Sorting sort_type) {
	Scoreboard scoreboard;
	char txt[SCREEN_WIDTH];

	// load scoreboard data
	if (loadScoreboard(&scoreboard) == 1) return;

	// Replace previous scoreboard by bg
	_sdl.drawRectangle(0, y, SCREEN_WIDTH, SCREEN_HEIGHT - y, _sdl.colors.legend_bg);

	// Draw Header
	_sdl.drawString(x, y, "TABLICA WYNIKOW:");
	y += 2 * 8;
	_sdl.drawString(x, y, "sortuj po liczbie punktow (klawisz p) lub po czasie (klawisz t)");

	// Sort
	sortScoreboard(scoreboard, sort_type);

	for (int i = skip; i < scoreboard.number_of_results; i++) {
		y += 2 * 8;
		sprintf(txt, "%d) Punkty: %.d   Czas: %.1lf s", i + 1, scoreboard.results[i].score, scoreboard.results[i].time);
		_sdl.drawString(x, y, txt);

		if (i >= skip + 14) break;
	}
}

void SpyHunter::saveToScoreboard() {
	Scoreboard scoreboard, new_scoreboard;
	Result new_result = { _score, _worldTime };

	// Prepare a new scoreboard
	if (loadScoreboard(&scoreboard) != 1) {
		// Create new larger results array and rewrite the values
		new_scoreboard.number_of_results = scoreboard.number_of_results + 1;
		new_scoreboard.results = new Result[scoreboard.number_of_results + 1];

		for (int i = 0; i < scoreboard.number_of_results; i++) {
			new_scoreboard.results[i] = scoreboard.results[i];
		}

		// Add new_result to results
		new_scoreboard.results[scoreboard.number_of_results] = new_result;

		delete[] scoreboard.results;
	}
	else {
		// Scoreboard.bin doesn't exist so this is the first result in our scoreboard
		new_scoreboard.number_of_results = 1;
		new_scoreboard.results = new Result[1];
		new_scoreboard.results[0] = new_result;
	};

	// Save new scoreboard to file
	FILE* file = fopen("./saves/scoreboard.bin", "wb");
	fwrite(&new_scoreboard.number_of_results, sizeof(int), 1, file);
	for (int i = 0; i < new_scoreboard.number_of_results; i++) {
		fwrite(&new_scoreboard.results[i], sizeof(Result), 1, file);
	}
	fclose(file);
	delete[] new_scoreboard.results;
}

int SpyHunter::loadScoreboard(struct Scoreboard* scoreboard) {
	FILE* file = fopen("./saves/scoreboard.bin", "rb");
	if (file == NULL) return 1;

	fread(&scoreboard->number_of_results, sizeof(int), 1, file);
	scoreboard->results = new Result[scoreboard->number_of_results];
	for (int i = 0; i < scoreboard->number_of_results; i++) {
		fread(&scoreboard->results[i], sizeof(Result), 1, file);
	}
	fclose(file);

	return 0;
}

void SpyHunter::sortScoreboard(Scoreboard& scoreboard, Sorting sort_type) {
	Result temp;

	for (int i = 0; i < scoreboard.number_of_results; i++) {
		for (int j = 0; j < scoreboard.number_of_results - 1; j++) {
			if (sort_type == by_points && scoreboard.results[j].score < scoreboard.results[j + 1].score) {
				temp = scoreboard.results[j];
				scoreboard.results[j] = scoreboard.results[j + 1];
				scoreboard.results[j + 1] = temp;
			}
			else if (sort_type == by_points && scoreboard.results[j].time < scoreboard.results[j + 1].time) {
				temp = scoreboard.results[j];
				scoreboard.results[j] = scoreboard.results[j + 1];
				scoreboard.results[j + 1] = temp;
			}
		}
	}
}