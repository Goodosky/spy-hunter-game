#pragma once
#include<math.h>
#include<stdio.h>
#include<string.h>

#ifndef SDL_CONTROLLER
#define SDL_CONTROLLER

extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}

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
		int legend_bg, road, grass;
	} colors;

	SdlController();

	SDL_Surface* loadBmp(const char* filepath);

	void updateTimeBasedValues();

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