#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "skin.h"
#include "common.h"

class Player
{
public:
	float x, y, speed;
	int face;
	Skin skin;
	SDL_Texture* texture[4];
	SDL_Rect dimensions;

	Player(float *sx, float *sy, float sspeed, int sface, Skin sskin) {
		x = *sx;
		y = *sy;
		face = sface;
		skin = sskin;
		speed = sspeed;

		for(int i = 0; i < 4; i++)
		{
			texture[i] = skin.texture[i];
		}
	}

	void render(SDL_Renderer* renderer)
	{
		dimensions.w = player_width;
		dimensions.h = player_height;

		dimensions.x = (int)(x - index_x_off) * tile_width;
		dimensions.y = (int)(y - index_y_off) * tile_height;

		SDL_RenderCopy(renderer, texture[face - 1], NULL, &dimensions);
	}

	~Player()
	{
		for(int i = 0; i < 4; i++)
		{
			SDL_DestroyTexture(texture[i]);
		}
	}
};

#endif // PLAYER_H
