#pragma once
#include "Animation.h"
#include "GraphicsSystem.h"
#include "GraphicsBufferManager.h"

/* Holds all data for a single unit
   by Hannah Fasco

   Currently unused but may be used in the future for win animations
*/

// TO-DO: Update so that backbuffer indices are loaded into the unit and not constants
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
	Vector2D mLocation;              // Screen location of the unit
	vector<Animation*> mAnimations;  // Vector of all animations available to the unit
	int mCurrentIndex;               // Current animation

	Vector2D mVelocity;              // Current velocity of unit
	int mSpeed;                      // Current speed of unit

	bool mIsFree;                    // Is unit allocated as part of the manager
	bool mDidAnimationChange;        // Did the animation change or not

	// Backbuffer indices; From original project
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