#pragma once
#include "GraphicsSystem.h"
#include "GameEvent.h"
#include "Font.h"
#include "Color.h"
#include <Timer.h>
#include <iostream>
#include <fstream>

// Done by Nate
// Loads all UI elements of game
// I did all aspects with the number options/notes
class HUD : public Trackable
{
public:
	HUD(GraphicsSystem& graphicsSystem, int dispWidth, int dispHeight);
	~HUD();

	void init(GraphicsBufferManager& graphicsBufferManager, int tileIndex, int gridSize, float tilePadding);
	
	void loadColorData(Color& text, Color& tile, Color& note);
	void loadFontData(string assetPath, string fontName, int menuFontSize, int noteFontSize);

	void update(float savedTime, bool notes);
	void pauseTimer();
	void reset();
	void loadData(string& filename);
	int checkInput(Vector2D loc);

	float getTime() { return mTime; }
	bool getNotesOn() { return mNotesOn; }
private:
	void draw();

	GraphicsSystem* mGraphicsSystem;
	GraphicsBufferManager* mGraphicsBufferManager;

	map<int, Vector2D> mNumMap;

	int mTileIndex;

	string mTimeText;
	string mNotesText;

	Timer mTimer;
	float mTime;

	int mGridSize;
	int mTileSize;
	float mTilePadding;

	float mStartNumLoc;
	Vector2D mNotesButtonLoc;
	
	bool mNotesOn;

	int mDispWidth;
	int mDispHeight;

	// Color variables
	Color mTextColor;
	Color mTileColor;
	Color mNoteUIColor;

	// Font variables
	Font mMenuFont;
	Font mNoteFont;
};