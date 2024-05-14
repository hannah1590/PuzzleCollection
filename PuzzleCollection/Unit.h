#pragma once
#include "Animation.h"
#include "GraphicsSystem.h"
#include "GraphicsBufferManager.h"

// TO-DO: Update so that backbuffer indices are loaded into the unit and not constants

// Holds all data for a single unit
class Unit : public Trackable
{
public:
	Unit(GraphicsBufferManager& graphicsBuffer, Vector2D location, Vector2D velocity, int speed);
	~Unit();

	void cleanup();

	void update(double frameRate);
	void draw(GraphicsSystem& pGraphicsSystem);

	void drawInvisible(GraphicsSystem& pGraphicsSystem);

	void setAnimation();
	void addAnimation(Animation& newAnimation) { mAnimations.push_back(&newAnimation); }

	void changeLocation(Vector2D newLocation) { mLocation = newLocation; }
	Vector2D getLocation() { return mLocation; }

	Animation* createAnimation(int index, GraphicsBufferManager& graphicsBuffer);

	void free() { mIsFree != mIsFree; }
	bool getFree() { return mIsFree; }

	void changeAnimation() { mDidAnimationChange = true; }
	bool getAnimationChange() { return mDidAnimationChange; }

	int getPixelWidth() { return PIXEL_WIDTH; }
	int getPixelHeight() { return PIXEL_HEIGHT; }
private:
	Vector2D mLocation;
	vector<Animation*> mAnimations;
	int mCurrentIndex;

	Vector2D mVelocity;
	int mSpeed;

	bool mIsFree;
	bool mDidAnimationChange;

	// Backbuffer indices
	const int BLACK_SCREEN_INDEX = 0;
	const int RED_BALL_INDEX = 1;
	const int BLUE_BALL_INDEX = 2;

	const int PIXEL_WIDTH = 32;
	const int PIXEL_HEIGHT = 32;
	const int SPRITES_ACROSS = 6;
	const int SPRITES_DOWN = 2;

	const int STARTING_SPRITE_DOWN = 4;
	const int STARTING_SPRITE_RIGHT = 6;
};