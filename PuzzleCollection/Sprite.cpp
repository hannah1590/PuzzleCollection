#include "Sprite.h"

// Loads the data for a new sprite
void Sprite::createSprite(GraphicsBuffer& pGraphicsBuffer, Vector2D pSourceLoc, Vector2D pSize)
{
	mGraphicsBuffer = &pGraphicsBuffer;
	mSourceLoc = pSourceLoc;
	mSize = pSize;
}