#pragma once
#include <Trackable.h>
#include <Vector2D.h>
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include "GraphicsSystem.h"
#include "Font.h"
#include "Color.h"

/* Controls all different game menus
   by Nate Spielman
   All aspects involving the current puzzle game done by Hannah Fasco
*/

class MenuManager : public Trackable
{
public:
	MenuManager(GraphicsSystem& graphicsSystem);
	~MenuManager();

	void loadColorData(Color& text);
	void loadFontData(string assetPath, string fontName, int menuFontSize, int smallMenuFontSize);

	// Getters
	bool shouldQuit() { return mShouldQuit; }
	bool getIsMenuOpen() { return mIsMenuOpen; }
	int getCurrentMenu() { return mCurrentMenu; }
	int getCurrentDifficulty() { return mCurrentDifficulty; }
	int getCurrentGridSize() { return mCurrentGridSize; }

	void draw();
	void toggleMenu();
	void loadData(string& filename);
	void checkInput(Vector2D loc);

	// Setters
	void setToMain() { mCurrentMenu = 0; }
	void setToGameOver() { mCurrentMenu = 2; }
	void setToWin(int score) { mCurrentMenu = 7; mScore = score; mIsMenuOpen = true; }
	void setCurrentDifficulty(int difficulty) { mCurrentDifficulty = difficulty; }

private:
	GraphicsSystem* mGraphicsSystem;

	bool mShouldQuit;              // If user is quitting game
	bool mIsMenuOpen;              // If a menu is open
	bool mIsSoundOn;               // If sound is turned on
	bool mIsGameStarted;           // Whether the game has been started or not
						           
	int mCurrentMenu;              // Current menu open
	int mCurrentDifficulty;        // Current difficulty selected
	int mCurrentGridSize;          // Current grid size selected

	map<string, string> mTextMap;  // Map that contains the text for each different menu

	int mScore;                    // Current game score

	Color mTextColor;              // Menu text color

	// Font variables
	string mAssetPath;             // Contains the file path to game assets
	string mFontName;              // Name of the font file 
	int mMenuFontSize;             // Basic UI font size
	int mSmallMenuFontSize;        // Smaller UI font size
	int mTextBuffer;               // Space in between menu text
};