#include "Tile.h"

Tile::Tile(Vector2D pos, int gridSize)
{
	mPosition = pos;
	mValue = 0;

	for (int i = 0; i < gridSize; i++)
	{
		mNotes[i] = false;
	}
}

Tile::~Tile()
{
	mNotes.clear();
}