#include "Animation.h"
Animation::Animation(bool shouldLoop)
{
	mTimePerSprite = 16.7; // 16.7 milliseconds per sprite achieves 60 fps
	mTimeRemaining = mTimePerSprite;
	mShouldLoop = shouldLoop;
	mCurrentSprite = 0;
}

// Cleans up mSprites
Animation::~Animation()
{
	mSprites.clear();
}

// Updates the sprites in the animation
void Animation::update(double frameRate)
{
	mTimeRemaining -= frameRate;
	// When mTimeRemaining in a sprite reaches 0 move to the next sprite
	if (mTimeRemaining <= 0) 
	{
		mCurrentSprite++;
		mTimeRemaining = mTimePerSprite;
		if (mCurrentSprite >= mSprites.size() && mShouldLoop)
			mCurrentSprite = 0;
	}
}