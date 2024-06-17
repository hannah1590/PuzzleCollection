#pragma once
#include <Trackable.h>
#include "GraphicsSystem.h"
#include "GraphicsBufferManager.h"
#include "Tile.h"
#include "GridFiller.h"
#include "GameEvent.h"
#include <map>
#include <iostream>

using namespace std;

/* Manages the current grid that is displayed on screen
   by Hannah Fasco
*/

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
	bool checkWinState();

	void updateRowState(int y);
	void updateColumnState(int x);
	void updateBoxState(int x, int y);

	void changeValue(int value);
	void addNote(int value) { mHighlightTile->turnOnOffNote(value); }
	void removeNotes();

	bool getIsHighlighting() { return mIsHighlighting; }
	Vector2D getHighlightGridPos() { return mHighlightTile->getGridPos(); }

	bool isRowCompleted(int y);
	bool isColumnCompleted(int x);
	bool isBoxCompleted(int x, int y);
private:
	// Pointer variables for game systems needed by class
	GraphicsSystem* mGraphicsSystem;
	GraphicsBufferManager* mGraphicsBufferManager;
	GridFiller* mGridFiller;

	map <int, Tile*> mGridMap;      // Map contains an index and a Tile for every spot on the grid
								    
	// Grid Variables			    
	int mGridSize;                  // Number of units the grid is; if 9x9 GridSize = 9
	int mBoxSizeX;                  // Number of units the individual boxes are on the x-axis; Both 9x9 and 6x6 have 3 units along the x-axis for each box
	int mBoxSizeY;                  // Number of units the individual boxes are on the y-axis; 9x9 has 3 units along the y-axis for each box while 6x6 usually has 2
	int mTileSize;                  // How big each tile is on the screen
	int mPercentRemoved;            // Percentage of board cleared
	float mTilePadding;             // Space between each tile
	float mNotePadding;             // Space between each noted number
								    
	GridType mCurrentGrid;          // Current game being played

	// Current tile variables
	bool mIsHighlighting = false;   // Whether a tile is currently selected
	Vector2D mHighlightLoc;         // Screen position of the selected tile
	Tile* mHighlightTile;           // Tile being selected

	// Score variables
	vector<int> mCompletedRows;     // Vector of all current completed rows; 0 is the top row and counts up as it goes down the rows
	vector<int> mCompletedColumns;  // Vector of all current completed columnes; 0 is the leftmost row and counts up as it across
	vector<int> mCompletedBoxes;    // Vector of all current completed boxes; 0 is the top left box and counts up left to right, then top to bottom

	// Color variables
	Color mDefaultNumberColor;      // Color for the numbers given at the start of each game
	Color mPlayerInputColor;        // Color for the numbers that the player inputs themselves
	Color mSameNumberColor;         // Color that numbers use when it is the same number as the one in the currently selected tile
	Color mWrongInputColor;         // Color for when the player inputs a wrong number

	// Font variables
	string mAssetPath;              // Contains the file path to game assets
	string mFontName;               // Name of the font file
	int mNumberFontSize;            // Size of the numbers in each tile
	int mNoteFontSize;              // Size of the notes displayed in each tile
};