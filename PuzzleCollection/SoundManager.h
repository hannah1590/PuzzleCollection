#pragma once
#include "AllegroInclude.h"
#include <Vector2D.h>
#include <map>
using namespace std;

class SoundManager:public Trackable
{
public:
	SoundManager(int maxSamples);
	~SoundManager();

	bool init();
	void loadSample(int index, const std::string& filename);
	void playSample(bool loop, int index);
	void destroySample(int index);
private:
	int mMaxNumOfSamples = 4;
	map<int, ALLEGRO_SAMPLE*> mSamples;
};