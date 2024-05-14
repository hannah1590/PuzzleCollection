#pragma once
#include "GraphicsSystem.h"
#include "InputSystem.h"
#include "Unit.h"
#include "UnitManager.h"
#include "GraphicsBufferManager.h"
#include "GameEvent.h"
#include "InputTranslator.h"
#include "HUD.h"
#include "SoundManager.h"
#include <EventListener.h>

#include "Timer.h"
#include "PerformanceTracker.h"
#include <random>
#include <fstream>

class GraphicsSystem;
class InputSystem;
class Unit;
class UnitManager;
class GraphicsBufferManager;
class InputTranslator;

class Game:public EventListener
{
public:
	static Game* getInstance();
	static void initInstance();
	static void cleanupInstance();

	void init();
	void cleanup();

	void createRandomUnit();

	void doLoop();
	void loadScreen(double targetTime);
	void handleEvent(const Event& theEvent);

	InputSystem* getInputSystem() { return mInputSystem; }

	void getVariables();
	void getSounds();
private:
	static Game* mspInstance;
	bool mIsInitted = false;

	GraphicsSystem* mGraphicsSystem;
	InputSystem* mInputSystem;
	UnitManager* mUnitManager;
	GraphicsBufferManager* mGraphicsBufferManager;
	InputTranslator* mInputTranslator;
	HUD* mHUD;
	SoundManager* mSoundManager;

	int mTimeElapsed = 0;
	int mTimePaused = 0;

	bool mIsLooping = true;
	bool mHasStarted = false;
	bool mHasPaused = false;

	int mMinVelocity = -100;
	int mMaxVelocity = 100;
	int mMinSpeed = 10;
	int mMaxSpeed = 60;

	int mMaxSpeedCap = 100;
	int mMinSpeedCap = 50;

	int mSpawnRadius = 0;
	int mSpawnRadiusCap = 50;

	int mRandomChance = 50;
	int mRandomChanceCap = 20;

	int mSecondsUntilIncrease = 10;

	int mSpeedIncreaseRate = 5;
	int mSpawnIncreaseRate = 5;
	int mRandomChanceIncreaseRate = 5;

	const int DISP_WIDTH = 800;
	const int DISP_HEIGHT = 600;

	const string FILE_PATH = "..\\..\PuzzleCollection\\PuzzleCollection\\";
	const string GAME_VARIABLES_FILENAME = "GameVariablesValues.txt";
	const string SOUND_FILENAMES_LOCATION = "SoundEffectsFileNames.txt";

	const string ASSET_PATH = "..\\..\\PuzzleCollection\\libraries\\assets\\";
	const string GLOW_BALLS_FILENAME = "glowing-balls.png";

	const string SOUND_ASSET_PATH = "minetrap\\";
	string mDeathSound = "KILLED.WAV";
	string mGameOverSound = "NEGFAN.WAV";
	string mChangeSpriteSound = "BLIP.WAV";
	string mSpawnSound = "POWERPK.WAV";

	int mDeathIndex = 0;
	int mGameOverIndex = 1;
	int mChangeSpriteIndex = 2;
	int mSpawnIndex = 3;
	int mMaxSamples = 4;
	
	const string FONT_FILENAME = "cour.ttf";
	const int FONT_SIZE = 50;

	int mPoints = 10;
	int mPointIncrease = 1;
	int mPointDecrease = 5;

	Game();
	~Game();
};