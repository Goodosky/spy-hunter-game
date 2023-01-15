#ifndef CONFIG
#define CONFIG

extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}

// Structs
struct RoadData {
	int start, end;
};

struct Color {
	int r, g, b;
};

struct Sprite {
	double x, y;
	int lives;
	SDL_Surface* surface;
};

enum Turning {
	off,
	right,
	left,
};

// Screen
#define SCREEN_WIDTH 700
#define SCREEN_HEIGHT 500

// Legend
#define FPS_COUNTER_REFRESH_INTERVAL 0.5 // in seconds
#define LEGEND_OFFSET_X 100
#define LEGEND_OFFSET_Y 5

// Game - road
#define INITIAL_ROAD_START 150
#define INITIAL_ROAD_END SCREEN_WIDTH - 150
#define ROAD_TURN_SIZE 30
#define ROAD_LEVEL_HEIGHT 50 // (must be a divisor of SCREEN_HEIGHT)
#define ROAD_TURN_PROBABILITY 500 // 4 = 100% | the more, the lower the probability
#define ROADSIDE_WIDTH 30

// Game - player
#define TURN_SPEED 450

// Game - others
#define FRAME_CAP 60
#define MAP_REFRESH_INTERVAL 0.1 // in seconds

// Colors
const Color LEGEND_BG_COLOR = { 0x11, 0x11, 0xCC };
const Color ROAD_COLOR = { 0x00, 0x00, 0x00 };
const Color GRASS_COLOR = { 0x00, 0x9A, 0x17 };
const Color ROADSIDE_COLOR = { 0xab, 0x9d, 0x5b };

#endif