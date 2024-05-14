#include "Animation.h"
Animation::Animation(bool shouldLoop)
{
	mTimePerSprite = 16.7;
	mTimeRemaining = mTimePerSprite;
	mShouldLoop = shouldLoop;
	mCurrentSprite = 0;
}

Animation::~Animation()
{
	mSprites.clear();
}

void Animation::update(double frameRate)
{
	mTimeRemaining -= frameRate;
	if (mTimeRemaining <= 0) 
	{
		mCurrentSprite++;
		mTimeRemaining = mTimePerSprite;
		if (mCurrentSprite >= mSprites.size() && mShouldLoop)
			mCurrentSprite = 0;
	}
}