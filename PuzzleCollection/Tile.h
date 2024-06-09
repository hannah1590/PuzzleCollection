#pragma once
#include <Trackable.h>
#include <Vector2D.h>
#include <map>
#include <iostream>
#include "Color.h"
#include "GraphicsSystem.h"
#include "GraphicsBufferManager.h"

using namespace std;

class Tile : public Trackable
{
public:
	Tile(Vector2D pos, Vector2D gridPos, int gridSize);
	~Tile();

	void init(GraphicsSystem& graphicsSystem, GraphicsBufferManager& graphicsBufferManager, float tilePadding);

	void loadColorData(Color& default, Color& playerInput, Color& sameNumber, Color& wrong);
	void loadFontData(Font& numberFont, Font& notesFont);

	Vector2D getPos() { return mPosition; }
	Vector2D getGridPos() { return mGridPos; }
	int getValue() { return mValue; }
	map<int, bool> getNotes() { return mNotes; }
	bool getDefault() { return mDefault; }
	bool getIsWrong() { return mIsWrong; }

	void changeValue(int value) { mValue = value; }
	void setIsWrong(bool wrong) { mIsWrong = wrong; }
	void changeFontColor(Color& color);
	void setDefault(); 

	void turnOnOffNote(int value);
	void draw(int bufferIndex);
private:
	GraphicsSystem* mGraphicsSystem;
	GraphicsBufferManager* mGraphicsBufferManager;

	Vector2D mPosition;
	Vector2D mGridPos;
	float mTilePadding;

	int mValue; // Actual number in the tile; 0 = no number
	map<int, bool> mNotes; // Notes the player assigns the tile for what possible numbers it can be
	
	bool mDefault; // Whether it is a default number that was assigned at the start or not
	bool mIsWrong = false; // If value is wrong

	// Color variables
	Color mDefaultNumberColor;
	Color mPlayerInputColor;
	Color mSameNumberColor;
	Color mWrongNumberColor;

	Color mCurrentColor;
	Color mOriginalColor;

	// Font variables
	Font mNumberFont;
	Font mNotesFont;
	const float NOTE_SPACING = 15.0f;
};