#pragma once
#include "GraphicsSystem.h"
#include "GraphicsBufferManager.h"
#include "InputSystem.h"
#include "InputTranslator.h"
#include "GameEvent.h"
#include "SoundManager.h"
#include "MenuManager.h"
#include "HUD.h"
#include "UnitManager.h"
#include "GridFiller.h"
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
class HUD;

// Controls all game systems
class Game : public EventListener
{
public:
	static Game* getInstance();
	static void initInstance();
	static void cleanupInstance();

	void init();
	void cleanup();

	void save();
	void loadSave();
	void getVariables();
	void getSounds();
	void getColors();
	void getFontData();

	void doLoop();
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
	GridFiller* mGridFiller;
	GridManager* mGridManager;

	int mTimeElapsed = 0;
	int mTimePaused = 0;

	bool mIsLooping = true;
	bool mHasStarted = false;
	bool mHasPaused = false;

	float mSavedTime;
	bool mNotesOn = false;

	// Game Variables
	int mGridSize;
	int mBoxSizeX;
	int mBoxSizeY;
	int mTileSize;
	int mPercentRemoved; // Percentage of board cleared
	float mTilePadding;
	float mNotePadding;

	const int DISP_WIDTH = 800;
	const int DISP_HEIGHT = 600;

	// File paths
	const string MAIN_FILE_PATH = "..\\..\\PuzzleCollection\\PuzzleCollection\\DataFiles\\";
	const string ASSET_FILE_PATH = "..\\..\\PuzzleCollection\\libraries\\assets\\";
	const string SOUND_ASSET_FILE_PATH = "minetrap\\";

	// File names
	const string GAME_VARIABLES_FILE = "GameVariables.txt";
	const string SOUND_LIST_FILE = "SoundEffects.txt";
	const string MENU_TEXT_FILE = "MenuData.txt";
	const string COLOR_DATA_FILE = "ColorData.txt";
	const string FONT_DATA_FILE = "FontData.txt";

	// Sounds
	string mDeathSound;
	string mGameOverSound;
	string mChangeSpriteSound;
	string mSpawnSound;
	int mMaxSamples;

	// Colors
	Color mBackgroundColor;
	Color mTileColor;
	Color mTextColor;
	Color mBorderColor;
	Color mHighlightColor;
	Color mDefaultNumberColor;
	Color mPlayerInputNumberColor;
	Color mSameNumberColor;
	Color mWrongInputColor;
	Color mNoteUIColor;

	// Font
	string mFontName;
	int mMenuFontSize;
	int mSmallMenuFontSize;
	int mNumberFontSize;
	int mNoteFontSize;

	// Index for each sound effect
	int mDeathIndex = 0;
	int mGameOverIndex = 1;
	int mChangeSpriteIndex = 2;
	int mSpawnIndex = 3;

	// Buffer Indices
	int mBackgroundIndex;
	int mTileIndex;
	int mXSeparatorIndex;
	int mYSeparatorIndex;
	int mXHighlightIndex;
	int mYHighlightIndex;

	Game();
	~Game();
};