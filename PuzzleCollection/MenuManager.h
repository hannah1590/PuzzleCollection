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

// Done by Nate

class MenuManager : public Trackable
{
public:
	MenuManager(GraphicsSystem& graphicsSystem);
	~MenuManager();

	void loadColorData(Color& text);
	void loadFontData(string assetPath, string fontName, int menuFontSize, int smallMenuFontSize);

	bool shouldQuit() { return mShouldQuit; }
	bool getIsMenuOpen() { return mIsMenuOpen; }
	int getCurrentMenu() { return mCurrentMenu; }
	int getCurrentDifficulty() { return mCurrentDifficulty; }
	int getCurrentGridSize() { return mCurrentGridSize; }

	void draw();
	void toggleMenu();
	void loadData(string& filename);
	void checkInput(Vector2D loc);

	void setToMain() { mCurrentMenu = 0; }
	void setToGameOver() { mCurrentMenu = 2; }
	void setToWin() { mCurrentMenu = 7; }
	void setCurrentDifficulty(int difficulty) { mCurrentDifficulty = difficulty; }

private:
	GraphicsSystem* mGraphicsSystem;
	bool mShouldQuit;
	bool mIsMenuOpen;
	bool mIsSoundOn;

	int mCurrentMenu;
	int mCurrentDifficulty;
	int mCurrentGridSize;

	map<string, string> mTextMap;

	// Color variables
	Color mTextColor;

	// Font variables
	string mAssetPath; 
	string mFontName;
	int mMenuFontSize;
	int mSmallMenuFontSize;
	int mTextBuffer;
};