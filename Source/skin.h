#ifndef SKIN_H
#define SKIN_H

#include <SDL2/SDL.h>

struct Skin
{
	char name[16];
	SDL_Texture* texture[4];
	int id;
};

#endif // SKIN_H
