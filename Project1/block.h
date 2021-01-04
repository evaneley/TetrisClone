/** \file */
#pragma once
#include "SDL.h"

/** Block class - What draws the blocks*/
class Block {
public:
	/**
	* enum Type
	* \brief used to know what kind of block is being used
	*/
	enum Type { iblock = 0, jblock, lblock, oblock, sblock, tblock, zblock };

	/**
	* Constructor
	* \brief Initializes type, coordinates, and rotation
	*/
	Block(Type);

	/**
	* shape
	* \brief Draws the block on the gamespace
	* \param SDL_Renderer *color - Renderer with a color set to the unique color of the block
	*/
	void shape(SDL_Renderer *color);

	/**
	* hold_shape
	* \brief Draws the hold block
	* \param SDL_Renderer *color - Renderer with a color set to the unique color of the block
	*/
	void hold_shape(SDL_Renderer *color);

	/**
	* preview_shape
	* \brief Draws the preview blocks
	* \param SDL_Renderer *color - Renderer with a color set to the unique color of the block
	* \param int pos - Where in the deque the block is
	*/
	void preview_shape(SDL_Renderer *color, int pos);

	/**
	* is_Block
	* \brief Checks if the current spot contains a block
	* \param int x - x coordinate of space
	* \param int y - y coordinate of space
	* \return Returns true if the space is occupied by a block
	*/
	bool isBlock(int x, int y) const;

	/**
	* fall
	* \brief Moves the block down, left, or right
	* \param int new_x - left or right movement of block
	* \param int new_y - up or down movement of block
	* \return Returns true if the space is occupied by a block
	*/
	void fall(int new_x, int new_y);

	/**
	* rotate
	* \brief rotates the block 90 degrees
	*/
	void rotate();

	//Accessors
	int get_x_coord() const { return x_coord; }
	int get_y_coord() const { return y_coord; }

	//Mutators
	void set_x(int x) { x_coord = x; }
	void set_y(int y) { y_coord = y; }
	void set_rotation(int r) { rotation = r; }

private:
	Type block; /**< Which shape the block is*/
	int x_coord; /**< x coordinate of the block*/
	int y_coord; /**< y coordinate of the block*/
	int rotation; /**< rotation of the block*/

};