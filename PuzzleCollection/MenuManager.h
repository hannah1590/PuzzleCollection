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

	bool shouldQuit() { return mShouldQuit; }
	bool getIsMenuOpen() { return mIsMenuOpen; }
	int getCurrentMenu() { return mCurrentMenu; }
	int getCurrentDifficulty() { return mCurrentDifficulty; }

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

	int mCurrentMenu;
	int mCurrentDifficulty;

	map<string, string> mTextMap;

	const string ASSET_PATH = "..\\..\\PuzzleCollection\\libraries\\assets\\";
	const string FONT_FILENAME = "cour.ttf";
	const int FONT_SIZE = 50;
	const int FONT_SIZE_SMALL = 25;
};