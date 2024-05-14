#include "Unit.h"

Unit::Unit(GraphicsBufferManager& graphicsBuffer, Vector2D location, Vector2D velocity, int speed)
{
	mIsFree = false;
	mDidAnimationChange = false;
	mLocation = location;
	mVelocity = velocity.getNormalizedVector();
	mVelocity *= speed;
	mSpeed = mSpeed;
	int mCurrentIndex = 0;

	addAnimation(*createAnimation(RED_BALL_INDEX, graphicsBuffer));
	addAnimation(*createAnimation(BLUE_BALL_INDEX, graphicsBuffer));
}

// TO-DO: make animations in animation class instead
// Makes an animation that contains each sprite
Animation* Unit::createAnimation(int index, GraphicsBufferManager& graphicsBuffer)
{
	Sprite sprite;
	Animation* animation = new Animation(true);
	int yMultiply = -1;

	for (int i = 0; i < SPRITES_ACROSS * SPRITES_DOWN; i++)
	{
		// Moves to next column
		int xPlacement = i % SPRITES_ACROSS;

		// Moves to next row
		if (i % SPRITES_ACROSS == 0)
		{
			yMultiply++;
		}
		if (index == RED_BALL_INDEX)
		{
			sprite.createSprite(*graphicsBuffer.getBuffer(index), Vector2D(PIXEL_WIDTH * (xPlacement), PIXEL_HEIGHT * (yMultiply + STARTING_SPRITE_DOWN)), Vector2D(PIXEL_WIDTH, PIXEL_HEIGHT));
		}
		else if (index == BLUE_BALL_INDEX)
		{
			sprite.createSprite(*graphicsBuffer.getBuffer(index), Vector2D(PIXEL_WIDTH * (xPlacement + STARTING_SPRITE_RIGHT), PIXEL_HEIGHT * (yMultiply + STARTING_SPRITE_DOWN)), Vector2D(PIXEL_WIDTH, PIXEL_HEIGHT));
		}
		animation->addSprite(sprite);
	}

	return animation;
}

Unit::~Unit()
{
	cleanup();
}

// Deletes all animations for a Unit
void Unit::cleanup()
{
	for (auto i : mAnimations)
	{
		delete i;
		i = nullptr;
	}
	mAnimations.clear();
}

// Update the animation's current sprite and location based on velocity
void Unit::update(double frameRate)
{
	mAnimations[mCurrentIndex]->update(frameRate);

	mLocation += mVelocity * (frameRate / 1000);
}

// Draw the unit to the backbuffer
void Unit::draw(GraphicsSystem& pGraphicsSystem)
{
	Vector2D size = mAnimations[mCurrentIndex]->getCurrentSprite().getSize();
	pGraphicsSystem.drawBackbuffer(mLocation - Vector2D(size.getX() / 2, size.getY() / 2), mAnimations[mCurrentIndex]->getCurrentSprite(), 1.0f);
}

// Draw an invisible unit to prevent errors
void Unit::drawInvisible(GraphicsSystem& pGraphicsSystem)
{
	Vector2D size = mAnimations[mCurrentIndex]->getCurrentSprite().getSize();
	pGraphicsSystem.drawBackbuffer(mLocation - Vector2D(size.getX() / 2, size.getY() / 2), mAnimations[mCurrentIndex]->getCurrentSprite(), 0.0f);
}

// Change the current animation of the unit
void Unit::setAnimation()
{
	mCurrentIndex++;
	if (mCurrentIndex >= mAnimations.size())
	{
		mCurrentIndex = 0;
	}
}
