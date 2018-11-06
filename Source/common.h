#ifndef COMMON_H
#define COMMON_H

#include <SDL2/SDL.h>
#include "skin.h"

const int screen_width = 1024;
const int screen_height = 768;

const int tile_width = 40;
const int tile_height = 40;

const int player_width = 40;
const int player_height = 40;

const int map_width = 128;
const int map_height = 128;

float index_x_off;
float index_y_off;

float render_off_x;
float render_off_y;

const int size_atlas = 255;
SDL_Texture* textures[size_atlas];

const int skin_count = 2;

Skin skins[skin_count];

#endif // COMMON_H
