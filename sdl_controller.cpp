#include "headers/sdl_controller.h"

SdlController::SdlController() {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		exit(1);
	}

	if (SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &_window, &_renderer) != 0) {
		SDL_Quit();
		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
		exit(1);
	};

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(_renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
	SDL_SetWindowTitle(_window, "Oskar Radziewicz 193676");

	screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
	_scrtex = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);

	// disable showing the cursor
	SDL_ShowCursor(SDL_DISABLE);

	// Load media
	_charset = loadBmp("./media/cs8x8.bmp");
	SDL_SetColorKey(_charset, true, 0x000000);

	// Assign colors
	_colors.black = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
	_colors.red = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
	_colors.green = SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00);
	_colors.blue = SDL_MapRGB(screen->format, 0x11, 0x11, 0xCC);

	// Set deflaut value for private variables
	_deltaTime = _worldTime = _fpsTimer = _fps = 0;
	_t1 = SDL_GetTicks();
	_frames = 0;
}


void SdlController::clearScreen() {
	SDL_FillRect(screen, NULL, _colors.black); // draw bg
}


void SdlController::refreshScreen() {
	SDL_UpdateTexture(_scrtex, NULL, screen->pixels, screen->pitch);
	//		SDL_RenderClear(renderer);
	SDL_RenderCopy(_renderer, _scrtex, NULL, NULL);
	SDL_RenderPresent(_renderer);
}


SDL_Surface* SdlController::loadBmp(const char* filepath) {
	// wczytanie obrazka cs8x8.bmp
	SDL_Surface* bmp = SDL_LoadBMP(filepath);

	if (bmp == NULL) {
		printf("SDL_LoadBMP(%s) error: %s\n", filepath, SDL_GetError());
		freeAllSurfacesAndQuit();
		exit(1);
	};

	return bmp;
}


void SdlController::updateTimeBasedValues() {

	_t2 = SDL_GetTicks();
	_deltaTime = (_t2 - _t1) * 0.001; // ms * 0.001 = seconds
	_t1 = _t2;

	_worldTime += _deltaTime;
	_fpsTimer += _deltaTime;

	if (_fpsTimer > FPS_COUNTER_REFRESH_INTERVAL) {
		_fps = _frames * (1 / FPS_COUNTER_REFRESH_INTERVAL);
		_frames = 0;
		_fpsTimer -= FPS_COUNTER_REFRESH_INTERVAL;
	};

	_frames++;
}


void SdlController::drawLegend() {
	drawRectangle(LEGEND_OFFSET_X, LEGEND_OFFSET_Y, SCREEN_WIDTH - 2 * LEGEND_OFFSET_X, 36, _colors.LEGEND_BG);

	sprintf(_legendText, "czas trwania = %.1lf s  %.0lf klatek / s", _worldTime, _fps);
	int x = screen->w / 2 - strlen(_legendText) * 8 / 2;
	int y = LEGEND_OFFSET_Y + 5;
	drawString(x, y, _legendText);

	sprintf(_legendText, "Esc - wyjscie, %c - lewo, %c - prawo", ARROW_LEFT, ARROW_RIGHT);
	x = screen->w / 2 - strlen(_legendText) * 8 / 2;
	y = LEGEND_OFFSET_Y + 21;
	drawString(x, y, _legendText);
}


void SdlController::drawString(int x, int y, const char* text) {
	int char_x, char_y, character_code;
	SDL_Rect src_rect, dest_rect;

	src_rect.w = dest_rect.w = 8;
	src_rect.h = dest_rect.h = 8;

	while (*text) {
		character_code = *text & 255;
		char_x = (character_code % 16) * 8;
		char_y = (character_code / 16) * 8;
		src_rect.x = char_x;
		src_rect.y = char_y;
		dest_rect.x = x;
		dest_rect.y = y;
		SDL_BlitSurface(_charset, &src_rect, screen, &dest_rect);
		x += 8;
		text++;
	};
}


void SdlController::drawSprite(SDL_Surface* sprite, int x, int y) {
	int offset_x = sprite->w / 2;
	int offset_y = sprite->h / 2;
	SDL_Rect dest_rect = { x - offset_x, y - offset_y, sprite->w, sprite->h };
	SDL_BlitSurface(sprite, NULL, screen, &dest_rect);
}


void SdlController::drawPixel(int x, int y, Uint32 color) {
	int bytes_per_pixel = screen->format->BytesPerPixel;
	Uint8* pixel_address = (Uint8*)screen->pixels + y * screen->pitch + x * bytes_per_pixel;
	*(Uint32*)pixel_address = color;
};


void SdlController::drawRectangle(int x, int y, int width, int height, Uint32 color) {
	SDL_Rect rect = { x, y, width, height };
	SDL_FillRect(screen, &rect, color);
}


void SdlController::freeAllSurfacesAndQuit() {
	SDL_FreeSurface(_charset);
	SDL_FreeSurface(screen);
	SDL_DestroyTexture(_scrtex);
	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window);

	SDL_Quit();
}