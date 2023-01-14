#ifndef CONFIG
#define CONFIG

// Screen
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

// Legend
#define FPS_COUNTER_REFRESH_INTERVAL 0.5 // in seconds
#define LEGEND_OFFSET_X 100
#define LEGEND_OFFSET_Y 5

// Game - road (everyting in px)
#define INITIAL_ROAD_START 150
#define INITIAL_ROAD_END SCREEN_WIDTH - 150
#define TURN_SIZE 20

// Colors
struct Color {
	int r, g, b;
};

const Color LEGEND_BG_COLOR = { 0x11, 0x11, 0xCC };
const Color ROAD_COLOR = { 0x00, 0x00, 0x00 };
const Color GRASS_COLOR = { 0x00, 0x9A, 0x17 };

#endif