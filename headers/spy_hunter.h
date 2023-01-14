#pragma once
#ifndef SPY_HUNTER
#define SPY_HUNTER

#include "headers/config.h"
#include "headers/sdl_controller.h"

class SpyHunter
{
private:
	SdlController _sdl;
	double _score, _delta;

	struct RoadData {
		int start, end;
	} _road[SCREEN_HEIGHT + 1];

public:
	SpyHunter();

	void startGame();

	void addRoadLevel();
	void drawRoad();
	void drawFrame();


	void endGame();
};

#endif