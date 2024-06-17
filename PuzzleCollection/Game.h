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

// Initial class declarations
class GraphicsSystem;
class InputSystem;
class Unit;
class UnitManager;
class GraphicsBufferManager;
class InputTranslator;
class HUD;
class GridManager;

/* Controls all game systems
   Most of the base and all aspects of the current puzzle game made by Hannah Fasco
   Contributions to Menu, HUD, Events, and other aspects done by Nate Spielman
*/

class Game : public EventListener
{
public:
	// Ensures a single instance of Game
	static Game* getInstance();
	static void initInstance();
	static void cleanupInstance();

	// Sets up and cleans important systems
	void init();
	void cleanup();

	// TO DO: Implement save functions
	void save();
	void loadSave();

	// Loads variable data from files
	void getVariables();
	void getColors();
	void getFontData();

	// Main game functions
	void doLoop();
	void handleEvent(const Event& theEvent);

	InputSystem* getInputSystem() { return mInputSystem; }
private:
	// Single Game instance
	static Game* mspInstance;
	bool mIsInitted = false;

	// Various pointers to each major game system
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

	// Timer variables
	int mTimeElapsed = 0;
	int mTimePaused = 0;
	float mSavedTime;
	bool mIsLooping = true;
	bool mHasStarted = false;
	bool mHasPaused = false;

	// Game Variables
	int mGridSize;           // Number of units the grid is; if 9x9 GridSize = 9
	int mBoxSizeX;           // Number of units the individual boxes are on the x-axis; Both 9x9 and 6x6 have 3 units along the x-axis for each box
	int mBoxSizeY;           // Number of units the individual boxes are on the y-axis; 9x9 has 3 units along the y-axis for each box while 6x6 usually has 2
	int mTileSize;           // How big each tile is on the screen
	int mPercentRemoved;     // Percentage of board cleared
	float mTilePadding;      // Space between each tile
	float mNotePadding;      // Space between each noted number
	int mScoreAdd;           // Amount of score added for each correct input
	int mScoreMinus;         // Amount of score subtracted for each incorrect input
	bool mNotesOn = false;   // Whether the notes function is on or off

	// Display variables
	const int DISP_WIDTH = 800;
	const int DISP_HEIGHT = 600;

	// File paths
	const string MAIN_FILE_PATH = "..\\..\\PuzzleCollection\\PuzzleCollection\\DataFiles\\";
	const string ASSET_FILE_PATH = "..\\..\\PuzzleCollection\\libraries\\assets\\";

	// File names
	const string GAME_VARIABLES_FILE = "GameVariables.txt";
	const string SOUND_LIST_FILE = "SoundEffects.txt";
	const string MENU_TEXT_FILE = "MenuData.txt";
	const string COLOR_DATA_FILE = "ColorData.txt";
	const string FONT_DATA_FILE = "FontData.txt";

	// TO DO: Implement sound into the game
	// Sounds
	string mRightInputSound;
	string mWrongInputSound;
	string mWinGameSound;
	int mMaxSamples;

	// Index for each sound effect
	int mRightInputIndex = 0;
	int mWrongInputIndex = 1;
	int mWinGameIndex = 2;
	int mMusicIndex = 3;

	// Colors
	Color mBackgroundColor;             // Color for the background screen
	Color mTileColor;                   // Color of each tile
	Color mTextColor;                   // Color of UI text 
	Color mBorderColor;                 // Color of the borders outlining the grid and each box
	Color mHighlightColor;              // Color that surrounds a tile when it is selected
	Color mDefaultNumberColor;          // Color for the numbers given at the start of each game
	Color mPlayerInputNumberColor;      // Color for the numbers that the player inputs themselves
	Color mSameNumberColor;             // Color that numbers use when it is the same number as the one in the currently selected tile
	Color mWrongInputColor;             // Color for when the player inputs a wrong number
	Color mNoteUIColor;                 // Color of the note numbers in each cell

	// Font
	string mFontName;                   // Font name found in file
	int mMenuFontSize;                  // Basic UI font size
	int mSmallMenuFontSize;             // Smaller UI font size
	int mNumberFontSize;                // Size of the numbers in the grid
	int mNoteFontSize;                  // Size of the note numbers

	// Buffer Indices
	int mBackgroundIndex;               // Buffer index that contains the basic background image; currently a black screen
	int mTileIndex;                     // Buffer index that makes each tile
	int mXSeparatorIndex;               // Buffer index for the outlines of the grid/boxes on the x-axis
	int mYSeparatorIndex;               // Buffer index for the outlines of the grid/boxes on the y-axis
	int mXHighlightIndex;               // Buffer index for the outline on the x-axis that surrounds the selected tile
	int mYHighlightIndex;               // Buffer index for the outline on the y-axis that surrounds the selected tile

	Game();
	~Game();
};