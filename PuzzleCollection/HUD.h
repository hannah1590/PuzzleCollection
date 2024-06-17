#pragma once
#include "GraphicsSystem.h"
#include "GameEvent.h"
#include "Font.h"
#include "Color.h"
#include <Timer.h>
#include <iostream>
#include <fstream>

/* Loads all UI elements of the game
   by Nate Spielman
   All aspects involving the current puzzle game done by Hannah Fasco
*/

class HUD : public Trackable
{
public:
	HUD(GraphicsSystem& graphicsSystem, int dispWidth, int dispHeight);
	~HUD();

	void init(GraphicsBufferManager& graphicsBufferManager, int tileIndex, int gridSize, float tilePadding);
	
	void loadColorData(Color& text, Color& tile, Color& note);
	void loadFontData(string assetPath, string fontName, int menuFontSize, int numberFontSize);
	void loadData(string& filename);

	void update(float savedTime, bool notes);
	void pauseTimer() { mTimer.pause(true); }
	void reset();

	int checkInput(Vector2D loc);

	float getTime() { return mTime; }
	bool getNotesOn() { return mNotesOn; }

	int getScore() { return mScore; }
	void addScore(int add) { mScore += add; }
	void minusScore(int minus) { mScore -= minus; if (mScore < 0) mScore = 0; }
private:
	void draw();

	// Pointers to game systems
	GraphicsSystem* mGraphicsSystem;
	GraphicsBufferManager* mGraphicsBufferManager;


	map<int, Vector2D> mNumMap;    // Map of each number tile displayed at the bottom of the screen

	int mTileIndex;                // Index for the tile buffer

	string mTimeText;              // Displays the time
	string mScoreText;             // Displays current score
	string mNotesText;             // Displays current notes for each tile

	Timer mTimer;                  // Holds the timer
	float mTime;                   // Current game time
	int mScore;                    // Current score 

	int mGridSize;                 // Number of units the grid is; if 9x9 GridSize = 9
	int mTileSize;                 // How big each tile is on the screen
	float mTilePadding;            // Space between each tile

	float mStartNumLoc;            // y value of where the number tiles start on the screen
	Vector2D mNotesButtonLoc;      // Location of the notes button
	
	bool mNotesOn;                 // Whether the notes are on or off

	int mDispWidth;                // Current display width
	int mDispHeight;               // Current display height

	// Color variables
	Color mTextColor;              // Color of UI text 
	Color mTileColor;              // Color of each tile
	Color mNoteUIColor;            // Color of the note numbers in each cell

	// Font variables
	string mAssetPath;             // Contains the file path to game assets
	string mFontName;              // Name of the font file
	int mMenuFontSize;             // Basic UI font size
	int mNumberFontSize;           // Size of the numbers in each tile
};