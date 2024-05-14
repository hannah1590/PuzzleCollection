#pragma once
#include "GraphicsBuffer.h"
class Sprite:public Trackable
{
public:
	Sprite() = default;
	void createSprite(GraphicsBuffer& pGraphicsBuffer, Vector2D pSourceLoc, Vector2D pSize);
	GraphicsBuffer* getGraphicsBuffer() { return mGraphicsBuffer; }
	Vector2D getSoureLoc() { return mSourceLoc; }
	Vector2D getSize() { return mSize; }
private:
	GraphicsBuffer* mGraphicsBuffer;
	Vector2D mSourceLoc;
	Vector2D mSize;
};