#pragma once
#ifndef SPY_HUNTER
#define SPY_HUNTER

#include "headers/config.h"
#include "headers/sdl_controller.h"

class SpyHunter
{
private:
	double _score, _delta, _speed, _elapsedMapUpdate;
	SdlController _sdl;
	RoadData _road[SCREEN_HEIGHT + 1];
	Sprite _player;
	Turning _turning = off;

public:
	SpyHunter();

	void startGame();

	void drawPlayer();
	void turn(Turning direction);

	void updateRoad();
	void addRoadLevel();
	void drawRoad();

	void drawFrame(bool is_resuming);
	void drawSprite(Sprite sprite);


	void endGame();
};


#endif