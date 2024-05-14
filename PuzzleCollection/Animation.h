#pragma once
#include "Sprite.h"
#include <vector>
class Animation : public Trackable
{
public:
	Animation(bool shouldLoop);
	~Animation();

	void addSprite(Sprite newSprite) { mSprites.push_back(newSprite); };
	void update(double frameRate);

	int getCurrentIndex() { return mCurrentSprite; }
	Sprite getCurrentSprite() { return mSprites[mCurrentSprite]; }
private:
	std::vector<Sprite> mSprites;  // contains every sprite in the animation
	double mTimePerSprite;         // time each sprite is on the screen in milliseconds
	double mTimeRemaining;         // how much time the sprite has remaining on screen
	int mCurrentSprite;            // current sprite on screen
	bool mShouldLoop;              // does the animation loop
};