/** \file */
#pragma once
#include "SDL.h"
#include "block.h"
#include "SDL_mixer.h"
#include <string>

/** Matrix class - Handles what happends in the gamespace */
class Matrix
{
public:
	/**
	* Constructor
	* \brief Clears all the gamespace and sets score to 0
	*/
	Matrix();

	/**
	* enum
	* \brief Sets a basic width and height of the gamespace
	*/
	enum { Width = 10, Height = 20 };

	/**
	* draw
	* \brief draws the gamespace
	* \param SDL_Renderer *renderer - Renderer for where to draw the gamespace
	*/
	void draw(SDL_Renderer *renderer);

	/**
	* chk_collision
	* \brief checks if a block is colliding with another block already placed
	* \param const Block &b - block that is being checked
	*/
	bool chk_collision(const Block &b) const;

	/**
	* unite
	* \brief places the block being played onto the gamespace
	* \param const Block &b - block that is being placed
	*/
	void unite(const Block &b);

private:
	bool data[Width][Height]; /**< 2D Array to keep track of where blocks/spaces are */
	Mix_Chunk *clear_fx; /**< Chunk for clear sound effect */
	bool cleared_line = false; /**< bool to know if a line was cleared */
	int score; /**< int to keep track of player's score */
	std::string score_str = std::to_string(score); /**< String of the score to display it using ttf */
};


