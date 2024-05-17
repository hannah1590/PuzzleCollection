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
#include "MenuManager.h"
#include "GridManager.h"
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

// Controls all game systems
class Game : public EventListener
{
public:
	static Game* getInstance();
	static void initInstance();
	static void cleanupInstance();

	void init();
	void cleanup();

	void createRandomUnit();

	void save();
	void loadSave();
	void getVariables();
	void getSounds();

	void doLoop();
	void loadScreen(double targetTime);
	void handleEvent(const Event& theEvent);

	InputSystem* getInputSystem() { return mInputSystem; }
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
	MenuManager* mMenuManager;
	GridManager* mGridManager;

	int mTimeElapsed = 0;
	int mTimePaused = 0;

	bool mIsLooping = true;
	bool mHasStarted = false;
	bool mHasPaused = false;

	// Variables from original project; Changable in GameVariableValues.txt
	int mMinVelocity;
	int mMaxVelocity;
	int mMinSpeed;
	int mMaxSpeed;

	int mMaxSpeedCap;
	int mMinSpeedCap;

	int mSpawnRadius;
	int mSpawnRadiusCap;

	int mRandomChance;
	int mRandomChanceCap;

	int mSecondsUntilIncrease;

	int mSpeedIncreaseRate;
	int mSpawnIncreaseRate;
	int mRandomChanceIncreaseRate;

	int mPoints;
	int mPointIncrease;
	int mPointDecrease;

	float mSavedTime;


	const int DISP_WIDTH = 800;
	const int DISP_HEIGHT = 600;

	// File paths; TO-DO: change to editable in tex doc 
	const string FILE_PATH = "..\\..\\PuzzleCollection\\PuzzleCollection\\";
	const string GAME_VARIABLES_FILENAME = "GameVariablesValues.txt";
	const string SOUND_FILENAMES_LOCATION = "SoundEffectsFileNames.txt";

	const string ASSET_PATH = "..\\..\\PuzzleCollection\\libraries\\assets\\";
	const string GLOW_BALLS_FILENAME = "glowing-balls.png";

	const string SOUND_ASSET_PATH = "minetrap\\";

	// Found in SoundEffectFileNames.txt
	string mDeathSound;
	string mGameOverSound;
	string mChangeSpriteSound;
	string mSpawnSound;
	int mMaxSamples;

	// Index for each sound effect
	int mDeathIndex = 0;
	int mGameOverIndex = 1;
	int mChangeSpriteIndex = 2;
	int mSpawnIndex = 3;

	Game();
	~Game();
};