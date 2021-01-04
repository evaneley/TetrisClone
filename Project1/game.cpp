#include "game.h"
#include <iostream>
#include <ctime>

Game::Game() :
	block{ static_cast<Block::Type>(rand() % 7) },
	move_time(SDL_GetTicks()),
	hold{ static_cast<Block::Type>(-1) }
{
	//Ensures game starts with a different block everytime
	srand(time(0));
	//Get first block
	block = static_cast<Block::Type>(rand() % 7);

	//Load deque
	for (size_t i = 0; i < 3; i++)
	{
		Block tmp = static_cast<Block::Type>(rand() % 7);
		dq.push_back(tmp);
	}

	//Initialize Video and audio
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	//Initialize font
	TTF_Init();

	//Create window and renderer
	SDL_CreateWindowAndRenderer(1080, 720, SDL_WINDOW_OPENGL, &window, &renderer);

	//Set the window title
	SDL_SetWindowTitle(window, "Tetris");

	//Open audio
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	//Load sound files
	bgm = Mix_LoadMUS("tetristheme.wav");
	egm = Mix_LoadMUS("Tetrepic.mid");
	drop_fx = Mix_LoadWAV("drop.wav");
	move_fx = Mix_LoadWAV("turn.wav");
	hold_fx = Mix_LoadWAV("hold.wav");
	matrix = Matrix(); //Calls the constructor to initialize clear audio

	//Start Background Music
	if (!Mix_PlayingMusic())
	{
		Mix_PlayMusic(bgm, -1);
	}
}

Game::~Game()
{
	//Free SDL
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	//Free Mixer
	Mix_Quit();
	Mix_FreeMusic(bgm);
	Mix_FreeMusic(egm);
	Mix_FreeChunk(drop_fx);
	Mix_FreeChunk(move_fx);
	Mix_FreeChunk(hold_fx);
}

bool Game::tick() {
	SDL_Event e; //event for controls/auto move down
	SDL_Event n; //event for game over choice
	bool end = false;

	//Set timeout to 250 for next event to occur
	//Means that one event can occur per 250 ms
	if (SDL_WaitEventTimeout(&e, 250))
	{
		switch (e.type)
		{
		case SDL_KEYDOWN:
		{
			switch (e.key.keysym.sym)
			{
			case SDLK_DOWN:
			{
				if (!pause && !end)
				{
					Block b = block;
					b.fall(0, 1);
					if (!matrix.chk_collision(b))
						block = b;
					Mix_PlayChannel(-1, move_fx, 0);
				}

			}
			break;

			case SDLK_LEFT:
			{
				if (!pause && !end)
				{
					Block b = block;
					b.fall(-1, 0);
					if (!matrix.chk_collision(b))
						block = b;
					Mix_PlayChannel(-1, move_fx, 0);
				}

			}
			break;

			case SDLK_RIGHT:
			{
				if (!pause && !end)
				{
					Block b = block;
					b.fall(1, 0);
					if (!matrix.chk_collision(b))
						block = b;
					Mix_PlayChannel(-1, move_fx, 0);
				}

			}
			break;

			case SDLK_UP:
			{
				if (!pause && !end)
				{
					Block b = block;
					b.rotate();
					if (!matrix.chk_collision(b))
						block = b;
					Mix_PlayChannel(-1, move_fx, 0);
				}

			}
			break;

			case SDLK_SPACE:
			{
				if (!pause && !end)
				{
					//Move block down until collision
					while (!matrix.chk_collision(block))
					{
						block.fall(0, 1);
					}
					//Needs moved up one because in the check, it auto falls down one
					block.fall(0, -1);
					//Decrease move time for instant update :D
					move_time -= 1000;
					Mix_PlayChannel(-1, drop_fx, 0);
				}


			}
			break;

			case SDLK_c:
			{
				if (!pause && !end)
				{
					if (!held_turn)
					{
						if (held)
						{
							//Swap hold and block
							Block tmp = block;
							block = hold;
							hold = tmp;

							//Set coords and rotation to defaults
							hold.set_x(10 / 2 - 4 / 2);
							hold.set_y(0);
							hold.set_rotation(0);
							block.set_rotation(0);
							held_turn = true;

							//Play hold_fx
							Mix_PlayChannel(-1, hold_fx, 0);
						}
						else {
							//Set hold and position
							hold = block;
							hold.set_x(10 / 2 - 4 / 2);
							hold.set_y(0);
							hold.set_rotation(0);

							//Get next block and update dq
							block = dq.front();
							dq.pop_front();
							dq.push_back(Block{ static_cast<Block::Type>(rand() % 7) });

							held_turn = true;
							//Play hold_fx
							Mix_PlayChannel(-1, hold_fx, 0);
						}
						//Update display
						SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xff);
						SDL_RenderClear(renderer);
						matrix.draw(renderer);
						block.shape(renderer);
						hold.hold_shape(renderer);
						SDL_RenderPresent(renderer);

						if (SDL_GetTicks() > move_time) {
							move_time += 1000;
							Block b = block;
							b.fall(0, 1);
							check_block(b, end);
						}
						SDL_RenderPresent(renderer);

						held = true;
					}
				}

			}
			break;




			case SDLK_ESCAPE:
				pause = !pause;
				if (pause)
				{
					Mix_PauseMusic();
				}
				else {
					Mix_ResumeMusic();
				}
				break;
			}
		}
		break;
		case SDL_QUIT:
			return false;
		}
	}

	//Update display
	if (pause || end)
	{
		if (SDL_GetTicks() > move_time) {
			move_time += 1000;
		}
	}

	else {
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xff);
		SDL_RenderClear(renderer);
		matrix.draw(renderer);
		block.shape(renderer);
		for (auto i = 0; i < 3; i++)
		{
			dq[i].preview_shape(renderer, i);
		}
		if (held)
		{
			hold.hold_shape(renderer);
		}

		if (SDL_GetTicks() > move_time) {
			move_time += 1000;
			Block b = block;
			b.fall(0, 1);
			check_block(b, end);
		}
		SDL_RenderPresent(renderer);
	}
	if (end) {

		Mix_PauseMusic();

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		Mix_PlayMusic(egm, -1);
		//Clear Board from Screen
		SDL_RenderClear(renderer);
		SDL_RenderPresent(renderer);


		//Draw Game Over Screen
		TTF_Font* font = TTF_OpenFont("8-BIT WONDER.ttf", 24);
		SDL_Color white = { 255, 255, 255, 255 };
		SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, "Game Over", white);
		SDL_Texture* overMessage = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
		SDL_Rect message_rect{ 250, 100, 600, 300 };


		SDL_RenderCopy(renderer, overMessage, NULL, &message_rect);
		SDL_RenderPresent(renderer);

		SDL_Surface* surfacesMessage = TTF_RenderText_Solid(font, "Press Y to Continue or X to Exit", white);
		SDL_Texture* continueMessage = SDL_CreateTextureFromSurface(renderer, surfacesMessage);
		SDL_Rect Message_rect{ 150, 500, 800, 100 };


		SDL_RenderCopy(renderer, continueMessage, NULL, &Message_rect);
		SDL_RenderPresent(renderer);

		//Infinite loop until selection is made
		while (1) {
			if (SDL_WaitEventTimeout(&n, 27500))
			{
				switch (n.type)
				{
					case SDL_KEYDOWN:
					{
						switch (n.key.keysym.sym)
						{
							case SDLK_y:
							{
								Mix_PauseMusic();
								Mix_PlayMusic(bgm, -1);
								move_time = SDL_GetTicks();
								return true;
							}
							break;
							case SDLK_x:
							{
								return false;
							}
							break;
						}
					}
				}
			}


		}
	}


	return true;
}

void Game::check_block(const Block &b, bool &end) {
	//See if the block has reached collision
	if (matrix.chk_collision(b)) {
		matrix.unite(b);
		//Get new block
		block = dq.front();
		block.set_rotation(0);
		dq.pop_front();
		dq.push_back(Block{ static_cast<Block::Type>(rand() % 7) });
		//Check if new block is already colliding
		if (matrix.chk_collision(block))
		{
			//Reset game
			matrix = Matrix();
			hold = static_cast<Block::Type>(-1);
			held = false;
			end = true;
		}
		//Reset held_turn
		held_turn = false;
	}
	else {
		block = b;
	}
}