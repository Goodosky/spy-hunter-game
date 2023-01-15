#pragma once
#include<math.h>
#include<stdio.h>
#include<string.h>

#ifndef SDL_CONTROLLER
#define SDL_CONTROLLER

class SdlController
{
private:
	char _legendText[128];
	int _t1, _t2, _frames;
	double _deltaTime, _worldTime, _fpsTimer, _fps;

	SDL_Surface* screen, * _eti, * _charset;
	SDL_Texture* _scrtex;
	SDL_Window* _window;
	SDL_Renderer* _renderer;

public:
	struct Colors {
		int legend_bg, road, grass, roadside;
	} colors;

	SdlController();

	SDL_Surface* loadBmp(const char* filepath);

	void updateDelta(bool is_resuming);
	void updateDeltaBasedValues();

	void clearScreen();
	void refreshScreen();

	void drawString(int x, int y, const char* text);
	void drawRectangle(int x, int y, int width, int height, Uint32 color);
	void drawSprite(SDL_Surface* sprite, int x, int y);
	void drawPixel(int x, int y, Uint32 color);

	void drawLegend(double score);

	double getDelta();

	void freeAllSurfacesAndQuit();
};


#endif