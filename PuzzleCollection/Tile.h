#pragma once
#include <Trackable.h>
#include <Vector2D.h>
#include <map>
#include <iostream>
#include "Color.h"
#include "GraphicsSystem.h"
#include "GraphicsBufferManager.h"

using namespace std;

/* Holds the data for a single tile
   by Hannnah Fasco
*/

class Tile : public Trackable
{
public:
	Tile(Vector2D pos, Vector2D gridPos, int gridSize);
	~Tile();

	void init(GraphicsSystem& graphicsSystem, GraphicsBufferManager& graphicsBufferManager, float tilePadding);

	void loadColorData(Color& default, Color& playerInput, Color& sameNumber, Color& wrong);
	void loadFontData(string assetPath, string fontName, int numberFontSize, int noteFontSize);

	// Getters
	Vector2D getPos() { return mPosition; }
	Vector2D getGridPos() { return mGridPos; }
	int getValue() { return mValue; }
	map<int, bool> getNotes() { return mNotes; }
	bool getDefault() { return mDefault; }
	bool getIsWrong() { return mIsWrong; }

	// Setters
	void changeValue(int value) { mValue = value; }
	void setIsWrong(bool wrong) { mIsWrong = wrong; }
	bool getNoteValue(int noteNum) { return mNotes[noteNum - 1]; }
	void changeFontColor(Color& color);
	void setDefault(); 

	void turnOnOffNote(int value);

	void draw(int bufferIndex);
private:
	GraphicsSystem* mGraphicsSystem;
	GraphicsBufferManager* mGraphicsBufferManager;

	Vector2D mPosition;                   // Screen position of the tile
	Vector2D mGridPos;                    // Grid position of the tile
	float mTilePadding;                   // Spacing between each tile

	int mValue;                           // Actual number in the tile; 0 = no number
	map<int, bool> mNotes;                // Notes the player assigns the tile for what possible numbers it can be
	
	bool mDefault;                        // Whether it is a default number that was assigned at the start or not
	bool mIsWrong = false;                // If value is wrong

	// Color variables
	Color mDefaultNumberColor;            // Color for the numbers given at the start of each game
	Color mPlayerInputColor;              // Color for the numbers that the player inputs themselves
	Color mSameNumberColor;               // Color that numbers use when it is the same number as the one in the currently selected tile
	Color mWrongNumberColor;              // Color for when the player inputs a wrong number

	Color mCurrentColor;                  // Current color of the number
	Color mOriginalColor;                 // Original color set for the number

	// Font variables
	string mAssetPath;                    // Contains the file path to game assets
	string mFontName;                     // Name of the font file
	int mNumberFontSize;                  // Size of the numbers in each tile
	int mNoteFontSize;                    // Size of the notes displayed in each tile
	const float NOTE_SPACING = 15.0f;     // Spacing in between notes
};