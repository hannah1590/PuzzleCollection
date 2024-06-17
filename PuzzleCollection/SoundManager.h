#pragma once
#include "AllegroInclude.h"
#include <Vector2D.h>
#include <map>
#include <iostream>
#include <fstream>
using namespace std;

/* Controls allegro audio and sound samples
   by Hannah Fasco
   Contributed to by Nate Spielman: mMusicIndex, mMusicSampleID, and the soundOn() soundOff() functions as well as improved other functions

   Not being used right now but will be added later
*/

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

	int mMaxNumOfSamples = 4;           // Number of sound assets being used
	bool mSoundIsOn;                    // Whether sound is active or not
	int mMusicIndex;                    // Index of the BGM
	map<int, ALLEGRO_SAMPLE*> mSamples; // Map of all audio samples
	ALLEGRO_SAMPLE_ID mMusicSampleID;   // Specific music sample
};