#include "SoundManager.h"
SoundManager::SoundManager()
{
	mSoundIsOn = true;
}

// Cleans and destroys all sounds
SoundManager::~SoundManager()
{
	for (auto i : mSamples)
	{
		destroySample(i.first);
	}
	mSamples.clear();
}

// Inits all allegro sound systems
bool SoundManager::init(string& fileName, string& filePath)
{
	mMaxNumOfSamples = getNumOfSamples(fileName);
	if (!al_install_audio())
	{
		cout << "ERROR - Audio not installed correctly\n";
		return false;
	}
	if (!al_init_acodec_addon())
	{
		cout << "ERROR - Acodec addon not initted\n";
		return false;
	}
	if (!al_reserve_samples(mMaxNumOfSamples))
	{
		cout << "ERROR - Samples not reserved\n";
		return false;
	}
	loadData(fileName, filePath);
	return true;
}

// Load sound data
void SoundManager::loadData(string& filename, string& filePath)
{
	ifstream soundFile(filename);
	if (!soundFile.is_open())
	{
		cout << "Error getting file";
	}

	int numOfSounds;
	soundFile >> numOfSounds;
	int counter = 0;
	while (!soundFile.eof())
	{
		string soundName;
		soundFile >> soundName;
		loadSample(counter, filePath + soundName);
		counter++;
	}
	soundFile.close();
}

// Gets sample number
int SoundManager::getNumOfSamples(string& filename)
{
	ifstream soundFile(filename);
	int numToReserve;

	soundFile >> numToReserve;
	soundFile.close();

	return numToReserve;
}

// Fills sample map
void SoundManager::loadSample(int index, const std::string& filename)
{
	mSamples.emplace(index, al_load_sample((filename).c_str()));
}

void SoundManager::playSample(bool isMusic, int index)
{
	if (mSoundIsOn)
	{
		if (isMusic)
		{
			al_play_sample(mSamples[index], 1.0f, ALLEGRO_AUDIO_PAN_NONE, 1.0f, ALLEGRO_PLAYMODE_LOOP, &mMusicSampleID);
			mMusicIndex = index;
		}
		else
		{
			al_play_sample(mSamples[index], 1.0f, ALLEGRO_AUDIO_PAN_NONE, 1.0f, ALLEGRO_PLAYMODE_ONCE, nullptr);
		}
	}
}

void SoundManager::destroySample(int index)
{
	al_destroy_sample(mSamples[index]);
}

void SoundManager::soundOn()
{
	if (!mSoundIsOn)
	{
		mSoundIsOn = true;
		al_play_sample(mSamples[mMusicIndex], 1.0f, ALLEGRO_AUDIO_PAN_NONE, 1.0f, ALLEGRO_PLAYMODE_LOOP, &mMusicSampleID);
	}
}

void SoundManager::soundOff()
{
	if (mSoundIsOn)
	{
		mSoundIsOn = false;
		al_stop_sample(&mMusicSampleID);
	}
}