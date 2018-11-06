#ifndef SDL_EXTEND_H
#define SDL_EXTEND_H

#include <SDL2/SDL.h>

void SDL_RenderDrawBox(SDL_Renderer *renderer, int x, int y, int width, int height)
{
	for (int i = 0; i < height; i++)
	{
		SDL_RenderDrawLine(renderer, x, i + y, x + width, i + y);
	}
}

#endif // SDL_EXTEND_H
