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
	void loadGrid(GridType gridType, int gridSize, int dispWidth, int dispHeight);
	void clearGrid();
	void draw(int boxSizeX, int boxSizeY, int xSeparatorIndex, int ySeparatorIndex, int tileIndex, int xHighlightIndex, int yHighlightIndex);
	int checkInput(Vector2D loc);

	void changeValue(int value) { mHighlightTile->changeValue(value); }
	void addNote(int value) { mHighlightTile->addNote(value); }
private:
	map <int, Tile*> mGridMap;
	GraphicsSystem* mGraphicsSystem;
	GraphicsBufferManager* mGraphicsBufferManager;
	GridFiller* mGridFiller;

	int mTileSize; // How big each tile is
	int mGridSize; // How big the grid is; usually sudoku is 9x9 so the size would be 9
	GridType mCurrentGrid;

	bool mIsHighlighting = false;
	Vector2D mHighlightLoc;
	Tile* mHighlightTile;

	const float PADDING = 5.0f; // Padding between grid tiles
	const string ASSET_PATH = "..\\..\\PuzzleCollection\\libraries\\assets\\";
	const string FONT_FILENAME = "cour.ttf";
	const int FONT_SIZE = 30;
};