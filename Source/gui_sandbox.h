#ifndef GUI_SANDBOX_H
#define GUI_SANDBOX_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "SDL_Extend.h"

void GUI_DrawWindow(SDL_Renderer *renderer, int x, int y, int w, int h, int col[4], const char* font_file, int f_size, SDL_Color font_color, const char* title)
{
	TTF_Font* font = TTF_OpenFont(font_file, f_size);
	SDL_Texture* texture;

	SDL_Surface* surface = TTF_RenderUTF8_Blended(font, title, font_color);
	texture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

	SDL_Rect size;

	Uint32* format;
	int* access;

	SDL_QueryTexture(texture, format, access, &size.w, &size.h);

	size.x = x + 5;
	size.y = y + 5;

	SDL_SetRenderDrawColor(renderer, col[0], col[1], col[2], col[3]);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_RenderDrawBox(renderer, x, y, w, h);

	SDL_RenderCopy(renderer, texture, NULL, &size);
	SDL_DestroyTexture(texture);

	TTF_CloseFont(font);

	SDL_RenderPresent(renderer);
}

void GUI_SelectorNoImage(SDL_Renderer *renderer, int x, int y, int w, int h, int w_col[4], SDL_Color t_col, const char*title_font, int title_font_size, const char* title, char* name, char* out)
{
	GUI_DrawWindow(renderer, x, y, w, h, w_col, title_font, title_font_size, t_col, title);
}

#endif // GUI_SANDBOX_H
