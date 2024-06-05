#include "Tile.h"

Tile::Tile(int width, int height)
{
	mWidth = width;
	mHeight = height;
	mValue = 0;
}

Tile::~Tile()
{
	mNotes.clear();
}