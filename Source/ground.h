#ifndef GROUND_H
#define GROUND_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "common.h"

class Ground
{
public:
	int x, y;
	unsigned char type;
	SDL_Rect dimensions;

	void init(int sx, int sy, unsigned char stype)
	{
		x = sx;
		y = sy;
		type = stype;
	}

	void render(SDL_Renderer* renderer, int screen_x, int screen_y)
	{
		dimensions.w = tile_width;
		dimensions.h = tile_height;

		dimensions.x = screen_x * tile_width - (int)render_off_x;
		dimensions.y = screen_y * tile_height - (int)render_off_y;

		SDL_RenderCopy(renderer, textures[type], NULL, &dimensions);
	}
};

#endif // GROUND_H
