#include "SoundManager.h"
SoundManager::SoundManager(int maxSamples)
{
	mMaxNumOfSamples = maxSamples;
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
bool SoundManager::init() 
{
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
	return true;
}

// Fills sample map
void SoundManager::loadSample(int index, const std::string& filename)
{
	mSamples.emplace(index, al_load_sample((filename).c_str()));
}

void SoundManager::playSample(bool loop, int index)
{
	if (loop)
	{
		al_play_sample(mSamples[index], 1.0f, ALLEGRO_AUDIO_PAN_NONE, 1.0f, ALLEGRO_PLAYMODE_LOOP, nullptr);
	}
	else
	{
		al_play_sample(mSamples[index], 1.0f, ALLEGRO_AUDIO_PAN_NONE, 1.0f, ALLEGRO_PLAYMODE_ONCE, nullptr);
	}
}

void SoundManager::destroySample(int index)
{
	al_destroy_sample(mSamples[index]);
}