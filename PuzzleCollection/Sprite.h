#pragma once
#include "GraphicsBuffer.h"

/* Holds the data for a single sprite
   by Hannah Fasco

   Currently unused but may be used in the future for win animations
*/

class Sprite : public Trackable
{
public:
	Sprite() = default;
	void createSprite(GraphicsBuffer& pGraphicsBuffer, Vector2D pSourceLoc, Vector2D pSize);

	// Getters
	GraphicsBuffer* getGraphicsBuffer() { return mGraphicsBuffer; }
	Vector2D getSoureLoc() { return mSourceLoc; }
	Vector2D getSize() { return mSize; }
private:
	GraphicsBuffer* mGraphicsBuffer;   // Sprite graphics buffer
	Vector2D mSourceLoc;               // Location of sprite
	Vector2D mSize;                    // Size of sprite
};