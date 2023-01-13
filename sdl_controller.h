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

// Screen
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

// Legend
#define LEGEND_BG blue // look at the Colors struct to check the available colors
#define FPS_COUNTER_REFRESH_INTERVAL 0.5 // in seconds
#define LEGEND_OFFSET_X 100
#define LEGEND_OFFSET_Y 5
#define ARROW_LEFT '\032'
#define ARROW_RIGHT '\033'

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

	struct Colors {
		int black;
		int red;
		int green;
		int blue;
	} _colors;

public:
	SdlController();

	SDL_Surface* loadBmp(const char* filepath);

	void freeAllSurfacesAndQuit();
	void updateTimeBasedValues();

	void clearScreen();
	void refreshScreen();

	void drawLegend();
	void drawString(int x, int y, const char* text);
	void drawRectangle(int x, int y, int width, int height, Uint32 color);
	void drawSprite(SDL_Surface* sprite, int x, int y);
	void drawPixel(int x, int y, Uint32 color);
};


#endif