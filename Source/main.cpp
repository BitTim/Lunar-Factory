//"Lunar Factory" is a game created by BitTim
//Copyright (c) BitTim 2018

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include <stdio.h>
#include "ground.h"
#include "perlin.h"
#include "gui_sandbox.h"
#include "common.h"
#include "building.h"
#include "player.h"
#include "skin.h"

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Event event;

SDL_Surface* texture_loader;

FILE* log_file;

bool quit = false;
int cam_width = screen_width / tile_width, cam_height = screen_height / tile_height;

float spawn_x = (float)map_width / 2.0;
float spawn_y = (float)map_height / 2.0;

float cam_x, cam_y;

int mouse_x, mouse_y;

bool window_opened = false;

Ground map[map_width * map_height];
Building buildings[map_width * map_height];

Player* player;

int last_building_slot = 0;
int last_ground_slot = 0;

int build_pos_x;
int build_pos_y;

void SDL_Finish()
{
	SDL_SetWindowFullscreen(window, 0);

	delete player;

	for(int i = 0; i < size_atlas; i++)
	{
		SDL_DestroyTexture(textures[i]);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}

void load_textures()
{
	//Load Textures
	texture_loader = IMG_Load("Data/Graphics/transparent.png");
	textures[0] = SDL_CreateTextureFromSurface(renderer, texture_loader);
	SDL_FreeSurface(texture_loader);

	texture_loader = IMG_Load("Data/Graphics/missing.png");
	textures[1] = SDL_CreateTextureFromSurface(renderer, texture_loader);
	SDL_FreeSurface(texture_loader);

	texture_loader = IMG_Load("Data/Graphics/cratar_none.png");
	textures[2] = SDL_CreateTextureFromSurface(renderer, texture_loader);
	SDL_FreeSurface(texture_loader);

	texture_loader = IMG_Load("Data/Graphics/cratar_small.png");
	textures[3] = SDL_CreateTextureFromSurface(renderer, texture_loader);
	SDL_FreeSurface(texture_loader);

	texture_loader = IMG_Load("Data/Graphics/cratar_medium.png");
	textures[4] = SDL_CreateTextureFromSurface(renderer, texture_loader);
	SDL_FreeSurface(texture_loader);

	texture_loader = IMG_Load("Data/Graphics/cratar_big.png");
	textures[5] = SDL_CreateTextureFromSurface(renderer, texture_loader);
	SDL_FreeSurface(texture_loader);

	texture_loader = IMG_Load("Data/Graphics/chest.png");
	textures[6] = SDL_CreateTextureFromSurface(renderer, texture_loader);
	SDL_FreeSurface(texture_loader);

	//Load Skins
	SDL_Texture* tmp_player[4];

	texture_loader = IMG_Load("Data/Graphics/Skins/default_north.png");
	tmp_player[0] = SDL_CreateTextureFromSurface(renderer, texture_loader);
	SDL_FreeSurface(texture_loader);

	texture_loader = IMG_Load("Data/Graphics/Skins/default_east.png");
	tmp_player[1] = SDL_CreateTextureFromSurface(renderer, texture_loader);
	SDL_FreeSurface(texture_loader);

	texture_loader = IMG_Load("Data/Graphics/Skins/default_south.png");
	tmp_player[2] = SDL_CreateTextureFromSurface(renderer, texture_loader);
	SDL_FreeSurface(texture_loader);

	texture_loader = IMG_Load("Data/Graphics/Skins/default_west.png");
	tmp_player[3] = SDL_CreateTextureFromSurface(renderer, texture_loader);
	SDL_FreeSurface(texture_loader);

	char name[16] = "Default";

	for(int i = 0; i < 16; i++)
	{
		skins[0].name[i] = name[i];
	}

	for(int i = 0; i < 4; i++)
	{
		skins[0].texture[i] = tmp_player[i];
	}

	skins[0].id = 0;

	/*for(int i = 0; i < 4; i++)
	{
		SDL_DestroyTexture(tmp_player[i]);
	}*/

	cam_x = spawn_x;
	cam_y = spawn_y;

	index_x_off = cam_x - (float)cam_width / 2.0;
	index_y_off = cam_y - (float)cam_height / 2.0;

	player = new Player(&spawn_x, &spawn_y, 0.2, 3, skins[0]);
}

void redraw_screen()
{
	cam_x = player->x;
	cam_y = player->y;

	index_x_off = cam_x - (float)cam_width / 2.0;
	index_y_off = cam_y - (float)cam_height / 2.0;

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	if (index_x_off < 1)
	{
		index_x_off = 1;
	}

	if (index_y_off < 1)
	{
		index_y_off = 1;
	}

	if (index_x_off > (float)map_width - cam_width)
	{
		index_x_off = (float)map_width - cam_width;
	}

	if (index_y_off > (float)map_height - cam_height)
	{
		index_y_off = (float)map_height - cam_height;
	}

	render_off_x = (index_x_off - (int)index_x_off) * tile_width;
	render_off_y = (index_y_off - (int)index_y_off) * tile_height;

	for (int j = -1; j < cam_height + 2; j++)
	{
		for (int i = -1; i < cam_width + 2; i++)
		{
			if ((cam_y + j >= 0 && cam_x + i >= 0) && (cam_y + j < map_height - 1 && cam_x + i < map_width - 1))
			{
				map[(j + (int)index_y_off) * map_width + i + (int)index_x_off].render(renderer, i, j);
				buildings[(j + (int)index_y_off) * map_width + i + (int)index_x_off].render(renderer, i, j);
			}
		}
	}

	player->render(renderer);
	SDL_RenderPresent(renderer);
}

int main(int argv, char *argc[])
{
	log_file = fopen("log.txt", "w");
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	IMG_Init(IMG_INIT_PNG);

	/*SDL_DisplayMode DM;
	int DI;

	SDL_GetCurrentDisplayMode(DI, &DM);

	std::cout << DM.w << std::endl;
	std::cout << DM.h << std::endl;*/

	window = SDL_CreateWindow("Lunar Factory", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height, 0);
	renderer = SDL_CreateRenderer(window, -1, 0);

	load_textures();

	for (int j = 0; j < map_height; j++)
	{
		for (int i = 0; i < map_width; i++)
		{
			int chance = rand() % (100 - 1 + 1) + 1;

			if (chance < 26 && chance > 0)
			{
				map[j * map_width + i].init(i, j, 2);
			}
			else if (chance < 51 && chance > 25)
			{
				map[j * map_width + i].init(i, j, 3);
			}
			else if (chance < 76 && chance > 50)
			{
				map[j * map_width + i].init(i, j, 4);
			}
			else if (chance < 101 && chance > 75)
			{
				map[j * map_width + i].init(i, j, 5);
			}
			else
			{
				map[j * map_width + i].init(i, j, 1);
			}
		}
	}

	redraw_screen();

	while (!quit)
	{
		SDL_PollEvent(&event);

		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_w:
				if(player->face == 1)
				{
					if (player->y > player->speed)
					{
						player->y -= player->speed;
					}
				}
				else
				{
					player->face = 1;
				}
				break;
			case SDLK_a:
				if(player->face == 4)
				{
					if (player->x > player->speed)
					{
					player->x -= player->speed;
					}
				}
				else
				{
					player->face = 4;
				}
				break;
			case SDLK_s:
				if(player->face == 3)
				{
					if (player->y < map_height - 1)
					{
						player->y += player->speed;
					}
				}
				else
				{
					player->face = 3;
				}
				break;
			case SDLK_d:
				if(player->face == 2)
				{
					if (player->x < map_width - 1)
					{
						player->x += player->speed;
					}
				}
				else
				{
					player->face = 2;
				}
				break;
			case SDLK_e:
				if (!window_opened) {
					window_opened = true;
				}
				else
				{
					window_opened = false;
				}

				break;
			case SDLK_ESCAPE:
				quit = true;
				break;
			}

			redraw_screen();

			if (window_opened)
			{
				int win_col[4] = { 63, 63, 63, 200 };

				SDL_Color title_col;

				title_col.r = 255;
				title_col.g = 255;
				title_col.b = 255;
				title_col.a = 255;

				GUI_SelectorNoImage(renderer, screen_width / 4, screen_height / 4, screen_width / 2, screen_height / 2, win_col, title_col, "Data/Fonts/title_font.ttf", 20, "Inventory", NULL, NULL);
			}

			break;
		case SDL_MOUSEBUTTONDOWN:
			switch (event.button.button) {
			case SDL_BUTTON_LEFT:
				SDL_GetMouseState(&mouse_x, &mouse_y);
				build_pos_x = mouse_x;
				build_pos_y = mouse_y;

				build_pos_x = (int)(mouse_x / cam_width);
				build_pos_y = (int)(mouse_y / cam_height);

				if (player->x != (float)((int)player->x))
				{
					buildings[(int)((player->y + index_y_off + build_pos_y) * map_width + player->x + index_x_off + 1 + build_pos_x)].init(32, 32, 0x06);
				}
				else if (player->y != (float)((int)player->y))
				{
					buildings[(int)((player->y + index_y_off + 1 + build_pos_y) * map_width + player->x + index_x_off + build_pos_x)].init(32, 32, 0x06);
				}
				else if (player->x != (float)((int)player->x) && player->y != (float)((int)player->y))
				{
					buildings[(int)((player->y + index_y_off + 1 + build_pos_y) * map_width + player->x + index_x_off + 1 + build_pos_x)].init(32, 32, 0x06);
				}
				else
				{
					buildings[(int)((player->y + index_y_off + build_pos_y) * map_width + player->x + index_x_off + build_pos_x)].init(32, 32, 0x06);
				}

				redraw_screen();
				break;
			case SDL_BUTTON_RIGHT:
				SDL_GetMouseState(&mouse_x, &mouse_y);
				build_pos_x = mouse_x;
				build_pos_y = mouse_y;

				build_pos_x = (int)(mouse_x / cam_width);
				build_pos_y = (int)(mouse_y / cam_height);

				if (player->x != (float)((int)player->x))
				{
					buildings[(int)((player->y + index_y_off + build_pos_y) * map_width + player->x + index_x_off + 1 + build_pos_x)].init(32, 32, 0x00);
				}
				else if (player->y != (float)((int)player->y))
				{
					buildings[(int)((player->y + index_y_off + 1 + build_pos_y) * map_width + player->x + index_x_off + build_pos_x)].init(32, 32, 0x00);
				}
				else if (player->x != (float)((int)player->x) && player->y != (float)((int)player->y))
				{
					buildings[(int)((player->y + index_y_off + 1 + build_pos_y) * map_width + player->x + index_x_off + 1 + build_pos_x)].init(32, 32, 0x00);
				}
				else
				{
					buildings[(int)((player->y + index_y_off + build_pos_y) * map_width + player->x + index_x_off + build_pos_x)].init(32, 32, 0x00);
				}

				redraw_screen();
				break;
			}

			break;
		}
	}

	SDL_Finish();
	return 0;
}
