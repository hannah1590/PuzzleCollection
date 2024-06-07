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

	void init(GraphicsBufferManager& graphicsBufferManager, int tileIndex, int gridSize);
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
	Timer mTimer;
	float mTime;
	int mGridSize;
	int mTileSize;
	float mStartNumLoc;
	Vector2D mNotesButtonLoc;
	bool mNotesOn;

	int mDispWidth;
	int mDispHeight;

	const float PADDING = 6.0f; // Padding between tiles
	const string ASSET_PATH = "..\\..\\PuzzleCollection\\libraries\\assets\\";
	const string FONT_FILENAME = "cour.ttf";
	const int FONT_SIZE = 50;
	const int NOTE_FONT_SIZE = 30;
};