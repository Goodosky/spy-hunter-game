#pragma once
#ifndef SPY_HUNTER
#define SPY_HUNTER

#include "headers/config.h"
#include "headers/sdl_controller.h"

class SpyHunter
{
private:
	SdlController _sdl;

	struct RoadData {
		int start = INITIAL_ROAD_START;
		int end = INITIAL_ROAD_END;
	} _road[SCREEN_HEIGHT + 1];

public:
	SpyHunter();

	void addRoadLevel();
	void drawRoad();
	void drawFrame();


	void endGame();
};

#endif