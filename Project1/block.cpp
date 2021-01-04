#include "block.h"

Block::Block(Type type) :
	block(type),
	x_coord(10 / 2 - 4 / 2),
	y_coord(0),
	rotation(0)
{}


void Block::shape(SDL_Renderer* color) {

	switch (block) {
	case iblock:
		SDL_SetRenderDrawColor(color, 183, 52, 45, 255);
		break;
	case jblock:
		SDL_SetRenderDrawColor(color, 252, 107, 17, 255);
		break;
	case lblock:
		SDL_SetRenderDrawColor(color, 138, 249, 2, 255);
		break;
	case oblock:
		SDL_SetRenderDrawColor(color, 27, 81, 135, 255);
		break;
	case sblock:
		SDL_SetRenderDrawColor(color, 153, 71, 209, 255);
		break;
	case tblock:
		SDL_SetRenderDrawColor(color, 249, 4, 237, 255);
		break;
	case zblock:
		SDL_SetRenderDrawColor(color, 198, 188, 190, 255);
		break;
	}

	//Draw the block
	for (size_t x = 0; x < 4; x++) {
		for (size_t y = 0; y < 4; y++)
		{
			if (isBlock(x, y)){
				SDL_Rect rect{((x + x_coord) * 720 / 2 / 10) + 361, (y + y_coord) * 720 / 2 / 10 + 1, 720 / 2 / 10 - 2, 720 / 2 / 10 - 2 };
				SDL_RenderFillRect(color, &rect);
			}
		}
	}
}

void Block::hold_shape(SDL_Renderer *color)
{
	switch (block) {
	case iblock:
		SDL_SetRenderDrawColor(color, 183, 52, 45, 255);
		rotation = 1; //So when in hold area, it sits upright
		break;
	case jblock:
		SDL_SetRenderDrawColor(color, 252, 107, 17, 255);
		break;
	case lblock:
		SDL_SetRenderDrawColor(color, 138, 249, 2, 255);
		break;
	case oblock:
		SDL_SetRenderDrawColor(color, 27, 81, 135, 255);
		break;
	case sblock:
		SDL_SetRenderDrawColor(color, 153, 71, 209, 255);
		break;
	case tblock:
		SDL_SetRenderDrawColor(color, 249, 4, 237, 255);
		break;
	case zblock:
		SDL_SetRenderDrawColor(color, 198, 188, 190, 255);
		break;
	}

	//Draw the block
	for (size_t x = 0; x < 4; x++) {
		for (size_t y = 0; y < 4; y++)
		{
			if (isBlock(x, y)) {
				SDL_Rect rect{ (x * 720 / 2 / 10) + 220, (y * 720 / 2 / 10 + 1) + 50, 720 / 2 / 10 - 2, 720 / 2 / 10 - 2 };
				SDL_RenderFillRect(color, &rect);
			}
		}
	}
}

void Block::preview_shape(SDL_Renderer *color, int pos)
{
	switch (block) {
	case iblock:
		SDL_SetRenderDrawColor(color, 183, 52, 45, 255);
		rotation = 1; //So it displays upright in preview
		break;
	case jblock:
		SDL_SetRenderDrawColor(color, 252, 107, 17, 255);
		break;
	case lblock:
		SDL_SetRenderDrawColor(color, 138, 249, 2, 255);
		break;
	case oblock:
		SDL_SetRenderDrawColor(color, 27, 81, 135, 255);
		break;
	case sblock:
		SDL_SetRenderDrawColor(color, 153, 71, 209, 255);
		break;
	case tblock:
		SDL_SetRenderDrawColor(color, 249, 4, 237, 255);
		break;
	case zblock:
		SDL_SetRenderDrawColor(color, 198, 188, 190, 255);
		break;
	}
	
	//Draw the block
	for (size_t x = 0; x < 4; x++) {
		for (size_t y = 0; y < 4; y++)
		{
			if (isBlock(x, y)) {
				SDL_Rect rect{ (x * 720 / 4 / 10) + 750, ((y + (4 * pos) + pos) * 720 / 4 / 10 + 1) + 50, 720 / 4 / 10 - 2, 720 / 4 / 10 - 2 };
				SDL_RenderFillRect(color, &rect);
			}
		}
	}
	
	
}

bool Block::isBlock(int x, int y) const {
	static const char* Profile[][4] = {
		{
			"    "
			"####"
			"    "
			"    ",
			" #  "
			" #  "
			" #  "
			" #  ",
			"    "
			"####"
			"    "
			"    ",
			" #  "
			" #  "
			" #  "
			" #  "
			
		},
		{
			"    "
			"  # "
			"  # "
			" ## ",
			"    "
			"#   "
			"### "
			"    ",
			"    "
			" ## "
			" #  "
			" #  ",
			"    "
			"### "
			"  # "
			"    "
		},
		{
			"    "
			" #  "
			" #  "
			" ## ",
			"    "
			"    "
			"### "
			"#   ",
			"    "
			"##  "
			" #  "
			" #  ",
			"    "
			"  # "
			"### "
			"    "
			
		},
		{
			"    "
			" ## "
			" ## "
			"    ",
			"    "
			" ## "
			" ## "
			"    ",
			"    "
			" ## "
			" ## "
			"    ",
			"    "
			" ## "
			" ## "
			"    "
		},
		{
			"    "
			" ## "
			"##  "
			"    ",
			" #  "
			" ## "
			"  # "
			"    ",
			"    "
			" ## "
			"##  "
			"    ",
			" #  "
			" ## "
			"  # "
			"    "
		},
		{
			"    "
			"### "
			" #  "
			"    ",
			" #  "
			"##  "
			" #  "
			"    ",
			"    "
			" #  "
			"### "
			"    ",
			" #  "
			" ## "
			" #  "
			"    "
		},
		{
			"    "
			"##  "
			" ## "
			"    ",
			"  # "
			" ## "
			" #  "
			"    ",
			"    "
			"##  "
			" ## "
			"    ",
			"  # "
			" ## "
			" #  "
			"    "
		},
	};
	return Profile[block][rotation][x + y * 4] == '#';
}
	

void Block::fall(int new_x, int new_y) {
	x_coord += new_x;
	y_coord += new_y;

}

void Block::rotate() {
	++rotation;

	if (rotation >= 4)
		rotation = 0;

}