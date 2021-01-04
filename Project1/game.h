#pragma once
/** \file */
#include "SDL.h"
#include "Matrix.h"
#include "block.h"
#include "SDL_mixer.h"
#include <deque>
#include <SDL_ttf.h>

/** The game class is what will control how the game is played */
class Game
{
public:
	/**
	* \brief Initializes private variables to default setting for the beginning of the game
	*/
	Game();

	/**
	* Destructor
	* \brief Clears used assete to return memory to the system
	*/
	~Game();

	/** 
	* tick()
	* \brief Main driver of the game.
	* Runs many opertations for the playing of the game.
	* \return Returns true if game is being played; false if the game is over
	*/
	bool tick();

	/**
	* check_block()
	* \brief Checks if a Block is colliding with another block and if the game is over
	* \param const Block &b - Block that is to be checked
	* \param bool &end - Bool that will end the game if the block collides at the top
	*/
	void check_block(const Block &b, bool &end);

private:
	Game(const Game &);
	Game &operator= (const Game &); /**< Makes it so game is cannot be copied */
	SDL_Window *window; /**< Window for SDL */
	SDL_Renderer *renderer; /**< Renderer for SDL */
	Matrix matrix; /**< Matrix for the gamespace */
	std::deque<Block> dq; /**< deque that stores the next few blocks*/
	Block block; /**< Block that is being played */
	Block hold; /**< Block being held */
	bool held = false; /**< bool for initial hold so game knows to replace block with a new block */
	bool held_turn = false; /**< bool for each turn so player can only hold once per turn */
	size_t move_time; /**< used to know when to update the display */
	bool pause = false; /**< bool to tell if the game is paused*/
	Mix_Music *bgm; /**< Music for background */
	Mix_Music *egm; /**< Music for Game Over */
	Mix_Chunk *drop_fx; /**< Chunk for hard drop sound */
	Mix_Chunk *move_fx; /**< Chunk for hard drop sound */
	Mix_Chunk *hold_fx; /**< Chunk for hold sound */
};

