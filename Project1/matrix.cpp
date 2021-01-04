#include "matrix.h"
#include <iostream>
#include <SDL_ttf.h>


Matrix::Matrix():
	data{ false }, //Ititialize all of data to false
	score(0)
{
	TTF_Init();
	//Load clear_fx
	clear_fx = Mix_LoadWAV("clear.wav");
	
}

void Matrix::draw(SDL_Renderer *renderer) {
	SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);

	//Draw Border
	//Left Side
	SDL_RenderDrawLine(renderer, 359, 0, 359, 720);
	SDL_RenderDrawLine(renderer, 358, 0, 358, 720);
	//Right Side
	SDL_RenderDrawLine(renderer, 721, 0, 721, 720);
	SDL_RenderDrawLine(renderer, 722, 0, 722, 720);

	//Draw Hold Box
	SDL_Rect h_box{ 190, 40, 36*4 + 20, 36*4 + 20 };
	SDL_RenderDrawRect(renderer, &h_box);

	//Draw Preview Box
	SDL_Rect p_box{ 730, 40, 36 * 2 + 20, 36 * 8 - 10};
	SDL_RenderDrawRect(renderer, &p_box);

	//Labels

	TTF_Font *font = TTF_OpenFont("8-BIT WONDER.ttf", 20);
	SDL_Color fnt_color = { 255, 255, 255, 255 };

	//Hold Label
	SDL_Surface *hold_surface = TTF_RenderText_Solid(font, "Hold", fnt_color);
	SDL_Texture *hold_texture = SDL_CreateTextureFromSurface(renderer, hold_surface);
	SDL_Rect hold_txt_rect = {235, 40 + 36 * 4 + 30 };
	SDL_QueryTexture(hold_texture, NULL, NULL, &hold_txt_rect.w, &hold_txt_rect.h);
	SDL_RenderCopy(renderer, hold_texture, NULL, &hold_txt_rect);
	
	//"Next" Label
	SDL_Surface *next_surface = TTF_RenderText_Solid(font, "Next", fnt_color);
	SDL_Texture *next_texture = SDL_CreateTextureFromSurface(renderer, next_surface);
	SDL_Rect next_txt_rect = { 740, 10};
	SDL_QueryTexture(next_texture, NULL, NULL, &next_txt_rect.w, &next_txt_rect.h);
	SDL_RenderCopy(renderer, next_texture, NULL, &next_txt_rect);

	//Score Display
	TTF_Font *score_font = TTF_OpenFont("8-BIT WONDER.ttf", 32);
	SDL_Surface *score_surface = TTF_RenderText_Solid(score_font, score_str.c_str(), fnt_color);
	SDL_Texture *score_texture = SDL_CreateTextureFromSurface(renderer, score_surface);
	SDL_Rect score_txt_rect = { 150, 600 };
	SDL_QueryTexture(score_texture, NULL, NULL, &score_txt_rect.w, &score_txt_rect.h);
	SDL_RenderCopy(renderer, score_texture, NULL, &score_txt_rect);

	//Score Box
	SDL_Rect sc_box = { 140, 600, 360 - 145, 40 };
	SDL_RenderDrawRect(renderer, &sc_box);

	//Score Label
	SDL_Surface *score_lbl_surface = TTF_RenderText_Solid(font, "Score", fnt_color);
	SDL_Texture *score_lbl_texture = SDL_CreateTextureFromSurface(renderer, score_lbl_surface);
	SDL_Rect score_lbl_txt_rect = { 200, 570 };
	SDL_QueryTexture(score_lbl_texture, NULL, NULL, &score_lbl_txt_rect.w, &score_lbl_txt_rect.h);
	SDL_RenderCopy(renderer, score_lbl_texture, NULL, &score_lbl_txt_rect);
	
	//Draw Game Space
	for (int x = 0; x < Width; ++x)
		for (int y = 0; y < Height; ++y)
		{
			if (data[x][y])
			{
				SDL_Rect rect{ (x * 720 / 2 / 10 + 1) + 360, y * 720 / 2 / 10 + 1, 720 / 2 / 10 - 2, 720 / 2 / 10 - 2 };
				SDL_RenderFillRect(renderer, &rect);
			}
			else
			{
				SDL_SetRenderDrawColor(renderer, 16, 16, 16, 255);
				SDL_Rect grid{ (x * 720 / 2 / 10) + 360, y * 720 / 2 / 10, 720 / 2 / 10, 720 / 2 / 10 };
				SDL_RenderDrawRect(renderer, &grid);
				SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
			}
				
		}
}

bool Matrix::chk_collision(const Block &b) const
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (b.isBlock(i, j)) {
				//Get coordinates on where the block is
				int x_w = b.get_x_coord() + i;
				int y_w = b.get_y_coord() + j;
				//Check if it has reached the bottom of the matrix
				if (x_w < 0 || x_w >= Width || y_w < 0 || y_w >= Height)
					return true;
				//Check if it is hitting another block
				if (data[x_w][y_w])
					return true;
			}
		}
	}
	return false;
}

void Matrix::unite(const Block &b)
{
	int score_switch = 0; // switch variable for multi-line clears

	//Update array to include the block
	for (int x = 0; x < 4; ++x) {
		for (int y = 0; y < 4; ++y) {
			if (b.isBlock(x, y)) {
				data[b.get_x_coord() + x][(b.get_y_coord() + y) - 1] = true;
			}	
		}
	}

	//Check for and clear any full lines
	for (int y = Height - 1; y >= 0; y--)
	{
		bool full = true; //variable to keep track of if the line is full
		do
		{
			for (int x = 0; x < Width; x++)
			{
				//Iterate through gamespace and check that there
				//is a block in each space across a line
				if (!data[x][y])
				{
					//If a block is not present, line is not full
					//Set full to false and exit loop
					full = false;
					break;
				}
			}

			//If full is true, line is full, needs cleared
			if (full) {
				//Move anything above down 1
				for (int z = y - 1; z >= 0; z--)
				{
					for (int x = 0; x < Width; x++)
					{
						data[x][z + 1] = data[x][z];
					}
				}

				for (int i = 0; i < Width; i++)
				{
					data[i][0] = false;
				}
				score_switch++;
				cleared_line = true;
			}
		} while (full);

		if (cleared_line)
		{
			Mix_PlayChannel(-1, clear_fx, 0);
			cleared_line = false;
		}


	}
	// scoring based on tertis original scales along with 10 for each block placed.
	switch (score_switch)
	{
	case 1:
		score += 50;
		break;
	case 2:
		score += 110;
		break;
	case 3:
		score += 310;
		break;
	case 4:
		score += 1210;
		break;

	default:
		score += 10;
		break;
	}
	score_switch = 0;

	//Update score
	score_str = std::to_string(score);
	

		
}

