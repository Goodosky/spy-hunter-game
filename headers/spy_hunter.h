#pragma once
#ifndef SPY_HUNTER
#define SPY_HUNTER

#include "headers/config.h"
#include "headers/sdl_controller.h"

class SpyHunter
{
private:
	double _score, _delta, _elapsedMapUpdate, _worldTime;
	bool _score_paused;
	SdlController _sdl;
	RoadData _road[SCREEN_HEIGHT];
	Sprite _player;
	Turning _turning = off;

public:
	SpyHunter();

	void startGame();
	void restartGame();
	void endGame();

	void drawPlayer();
	void turn(Turning direction);
	void handleCollisions();

	void drawRoad();
	void addRoadLevel();
	void updateRoad();

	void drawFrame(bool is_resuming);
	void drawSprite(Sprite sprite);

	void drawScoreboard(int x, int y, int skip = 0, Sorting sort_type = by_points);
	void saveToScoreboard();
	int loadScoreboard(Scoreboard* scoreboard);
	void sortScoreboard(Scoreboard& scoreboard, Sorting sort_type);
};

#endif