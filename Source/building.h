#ifndef BUILDING_H
#define BUILDING_H

#include <SDL2/SDL.h>
#include "common.h"

class Building
{
public:
	int w, h;
	unsigned char type;
	SDL_Rect dimensions;

	void init(int sw, int sh, unsigned char stype)
	{
		w = sw;
		h = sh;
		type = stype;
	}

	void render(SDL_Renderer* renderer, int screen_x, int screen_y)
	{
		dimensions.w = w;
		dimensions.h = h;

		dimensions.x = screen_x * tile_width - (int)render_off_x;
		dimensions.y = screen_y * tile_height - (int)render_off_y;

		SDL_RenderCopy(renderer, textures[(int)type], NULL, &dimensions);
	}
};

#endif // BUILDING_H
