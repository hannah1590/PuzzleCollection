#pragma once
#include "Sprite.h"
#include <vector>

/* Holds all the sprites that together makes a single animation
   by Hannah Fasco
 
   Currently unused but may be used in the future for win animations
*/

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
	std::vector<Sprite> mSprites;  // Contains every sprite in the animation
	double mTimePerSprite;         // Time each sprite is on the screen in milliseconds
	double mTimeRemaining;         // How much time the sprite has remaining on screen
	int mCurrentSprite;            // Current sprite on screen
	bool mShouldLoop;              // Does the animation loop
};