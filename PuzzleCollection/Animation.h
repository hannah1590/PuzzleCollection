#pragma once
#include "Sprite.h"
#include <vector>
class Animation:public Trackable
{
public:
	Animation(bool shouldLoop);
	~Animation();

	void addSprite(Sprite newSprite) { mSprites.push_back(newSprite); };
	void update(double frameRate);

	int getCurrentIndex() { return mCurrentSprite; }
	Sprite getCurrentSprite() { return mSprites[mCurrentSprite]; }
private:
	std::vector<Sprite> mSprites;
	double mTimePerSprite;
	double mTimeRemaining;
	int mCurrentSprite;
	bool mShouldLoop;
};