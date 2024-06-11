#pragma once
#include "AllegroInclude.h"
#include <Vector2D.h>
#include <map>
#include <iostream>
#include <fstream>
using namespace std;

// Controls allegro audio and sound samples
// I did basis, Nate added the mMusicIndex, mMusicSampleID, and the soundOn() soundOff() functions as well as improved other functions
class SoundManager : public Trackable
{
public:
	SoundManager();
	~SoundManager();

	bool init(string& filename, string& filePath);
	void loadData(string& filename, string& filePath);
	void loadSample(int index, const std::string& filename);
	void playSample(bool isMusic, int index);
	void destroySample(int index);
	void soundOn();
	void soundOff();
private:
	int getNumOfSamples(string& fileName);

	int mMaxNumOfSamples = 4;
	bool mSoundIsOn;
	int mMusicIndex;
	map<int, ALLEGRO_SAMPLE*> mSamples;
	ALLEGRO_SAMPLE_ID mMusicSampleID;
};