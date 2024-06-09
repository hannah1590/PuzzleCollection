#pragma once
#include <Trackable.h>
#include "GraphicsSystem.h"
#include "GraphicsBufferManager.h"
#include "Tile.h"
#include "GridFiller.h"
#include <map>
#include <iostream>

using namespace std;

// Used to differentiate between the different available games
enum GridType
{
	SUDOKU,
	NUM_GRID_TYPES
};

class GridManager : public Trackable
{
public:
	GridManager(GraphicsSystem& graphicsSystem, GridFiller& gridFiller);
	~GridManager();

	void init(GraphicsBufferManager& graphicsBufferManager, int tileIndex);
	
	void loadGridVariables(int gridSize, int boxSizeX, int boxSizeY, int percentRemoved, float tilePadding, float notePadding);
	void loadColorData(Color& default, Color& playerInput, Color& sameNumber, Color& wrong);
	void loadFontData(string assetPath, string fontName, int numberFontSize, int noteFontSize);
	
	void loadGrid(GridType gridType, int dispWidth, int dispHeight);

	void clearGrid();
	void draw(int xSeparatorIndex, int ySeparatorIndex, int tileIndex, int xHighlightIndex, int yHighlightIndex);
	int checkInput(Vector2D loc);

	void changeValue(int value);
	void addNote(int value) { mHighlightTile->turnOnOffNote(value); }
private:
	map <int, Tile*> mGridMap;
	GraphicsSystem* mGraphicsSystem;
	GraphicsBufferManager* mGraphicsBufferManager;
	GridFiller* mGridFiller;

	// Grid Variables
	int mGridSize; // How big the grid is; usually sudoku is 9x9 so the size would be 9
	int mBoxSizeX;
	int mBoxSizeY;
	int mTileSize;
	int mPercentRemoved; // Percentage of board cleared
	float mTilePadding;
	float mNotePadding;

	GridType mCurrentGrid;

	bool mIsHighlighting = false;
	Vector2D mHighlightLoc;
	Tile* mHighlightTile;

	// Color variables
	Color mDefaultNumberColor;
	Color mPlayerInputColor;
	Color mSameNumberColor; 
	Color mWrongInputColor;

	// Font variables
	Font mNumberFont;
	Font mNotesFont;
};