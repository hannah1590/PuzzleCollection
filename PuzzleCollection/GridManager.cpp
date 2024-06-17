#include "GridManager.h"

// Loads in main pointer variables needed
GridManager::GridManager(GraphicsSystem& graphicsSystem, GridFiller& gridFiller)
{
	mGraphicsSystem = &graphicsSystem;
	mGridFiller = &gridFiller;
}

// Cleans up grid map
GridManager::~GridManager()
{
	clearGrid();
}

// Establishes buffer variables
void GridManager::init(GraphicsBufferManager& graphicsBufferManager, int tileIndex)
{
	mGraphicsBufferManager = &graphicsBufferManager;
	mTileSize = mGraphicsBufferManager->getBuffer(tileIndex)->getSize().getX();
}

// Loads variables to properly make grid
void GridManager::loadGridVariables(int gridSize, int boxSizeX, int boxSizeY, int percentRemoved, float tilePadding, float notePadding)
{
	mGridSize = gridSize;
	mBoxSizeX = boxSizeX;
	mBoxSizeY = boxSizeY;
	mPercentRemoved = percentRemoved;
	mTilePadding = tilePadding;
	mNotePadding = notePadding;
}

// Loads all color variables used by class
void GridManager::loadColorData(Color& default, Color& playerInput, Color& sameNumber, Color& wrong)
{
	mDefaultNumberColor = default;
	mPlayerInputColor = playerInput;
	mSameNumberColor = sameNumber;
	mWrongInputColor = wrong;
}


// Loads all font data used by class
void GridManager::loadFontData(string assetPath, string fontName, int numberFontSize, int noteFontSize)
{
	mAssetPath = assetPath;
	mFontName = fontName;
	mNumberFontSize = numberFontSize;
	mNoteFontSize = noteFontSize;
}

// Creates the intial grid
void GridManager::loadGrid(GridType gridType, int dispWidth, int dispHeight)
{
	mCurrentGrid = gridType;

	// Centers the grid
	float startPosX = (dispWidth / 2) - ((mGridSize * (mTileSize + mTilePadding)) / 2);
	float startPosY = (dispHeight / 2) - ((mGridSize * (mTileSize + mTilePadding)) / 2);

	// Set up grid according to current game
	if (mCurrentGrid == SUDOKU)
	{
		// Generates a sudoku board
		mGridFiller->fillGrid();

		// Goes through entire grid
		for (int i = 0; i < mGridSize; i++)
		{
			for (int j = 0; j < mGridSize; j++)
			{
				// Makes a tile and adds it to the map
				Vector2D loc = Vector2D(j * (mTileSize + mTilePadding) + startPosX, i * (mTileSize + mTilePadding) + startPosY);
				Tile* tile = new Tile(loc, Vector2D(j, i), mGridSize);
				mGridMap[i * mGridSize + j] = tile;

				// Load tile data
				tile->init(*mGraphicsSystem, *mGraphicsBufferManager, mTilePadding);
				tile->loadColorData(mDefaultNumberColor, mPlayerInputColor, mSameNumberColor, mWrongInputColor);
				tile->loadFontData(mAssetPath, mFontName, mNumberFontSize, mNoteFontSize);
				
				// Gets a number between 100 and 1
				int isRemoved = rand() % 100 + 1;
				
				// If the number is equal or higher to the percent, it stays, else it becomes invisible on the board
				if (isRemoved >= mPercentRemoved)
				{
					mGridMap[i * mGridSize + j]->changeValue(mGridFiller->getValue(j, i));
					mGridMap[i * mGridSize + j]->setDefault();

					// Update grid state at the end of every row, column, and box
					if (j == mGridSize - 1)
						updateRowState(i);
					if (i == mGridSize - 1)
						updateColumnState(j);
					if (((j + 1) % mBoxSizeX == 0 && (i + 1) % mBoxSizeY == 0))
						updateBoxState(j, i);
				}
			}
		}
	}
}

// Cleans up grid map
void GridManager::clearGrid()
{
	for (auto& i : mGridMap)
	{
		delete i.second;
		i.second = nullptr;
	}
	mGridMap.clear();
}

// Draws the current state of the grid on the screen
void GridManager::draw(int xSeparatorIndex, int ySeparatorIndex, int tileIndex, int xHighlightIndex, int yHighlightIndex)
{
	Vector2D loc;

	// Draw lines on the x-axis between each box
	for (int i = 0; i < mGridSize / mBoxSizeY; i++)
	{
		// Finds location by getting the first tile in each row and then subtracting the padding so the line is drawn inbetween
		loc = mGridMap[i * mGridSize * mBoxSizeY]->getPos() - Vector2D(mTilePadding, mTilePadding);
		mGraphicsSystem->drawBackbuffer(loc, *mGraphicsBufferManager->getBuffer(xSeparatorIndex), 1.0f);
	}
	// Line at bottom of grid
	loc = mGridMap[(mGridSize - 1) * mGridSize]->getPos() + Vector2D(-mTilePadding, (float)mTileSize);
	mGraphicsSystem->drawBackbuffer(loc, *mGraphicsBufferManager->getBuffer(xSeparatorIndex), 1.0f);
	
	// Draw lines on the y-axis between each box
	for (int i = 0; i < mGridSize / mBoxSizeX; i++)
	{
		// Finds location by getting the first tile in each column and then subtracting the padding so the line is drawn inbetween
		loc = mGridMap[i * (mGridSize / mBoxSizeY)]->getPos() - Vector2D(mTilePadding, mTilePadding);
		mGraphicsSystem->drawBackbuffer(loc, *mGraphicsBufferManager->getBuffer(ySeparatorIndex), 1.0f);
	}
	// Line at far right of grid
	loc = mGridMap[(mGridSize - 1)]->getPos() + Vector2D((float)mTileSize, -mTilePadding);
	mGraphicsSystem->drawBackbuffer(loc, *mGraphicsBufferManager->getBuffer(ySeparatorIndex), 1.0f);

	// Draw each tile with the number
	for (int i = 0; i < mGridMap.size(); i++)
	{
		// Checks what color the number needs to be
		if (mGridMap[i]->getValue() != 0)
		{
			// Changes the number to green if it is the same as the selected tile
			if (mIsHighlighting && mGridMap[i]->getValue() == mHighlightTile->getValue() && !mGridMap[i]->getIsWrong())
				mGridMap[i]->changeFontColor(mSameNumberColor);
			else if (!mGridMap[i]->getIsWrong())
				mGridMap[i]->changeFontColor(Color());
		}

		// Draw tile
		mGridMap[i]->draw(tileIndex);
	}

	// Draw highlighted box if a number is currently clicked
	if (mIsHighlighting)
	{
		// Draw above and below selected box
		Vector2D loc = mHighlightLoc - Vector2D(mTilePadding, mTilePadding);
		mGraphicsSystem->drawBackbuffer(loc, *mGraphicsBufferManager->getBuffer(xHighlightIndex), 1.0f);
		loc = mHighlightLoc + Vector2D(-mTilePadding, (float)mTileSize);
		mGraphicsSystem->drawBackbuffer(loc, *mGraphicsBufferManager->getBuffer(xHighlightIndex), 1.0f);

		// Draw right and left selected box
		loc = mHighlightLoc - Vector2D(mTilePadding, mTilePadding);
		mGraphicsSystem->drawBackbuffer(loc, *mGraphicsBufferManager->getBuffer(yHighlightIndex), 1.0f);
		loc = mHighlightLoc + Vector2D((float)mTileSize, -mTilePadding);
		mGraphicsSystem->drawBackbuffer(loc, *mGraphicsBufferManager->getBuffer(yHighlightIndex), 1.0f);
	}
}

// Checks if the player clicked on a specific grid tile
int GridManager::checkInput(Vector2D loc)
{
	float x = loc.getX();
	float y = loc.getY();

	// Loops through each tile on the grid map
	for (auto& i : mGridMap)
	{
		Vector2D tileLoc = i.second->getPos();
		// Checks if mouse click is in bounds of a tile
		if (x >= tileLoc.getX() && x <= tileLoc.getX() + mTileSize)
		{
			if (y >= tileLoc.getY() && y <= tileLoc.getY() + mTileSize)
			{
				// If tile is in bounds, assign it as the currently selected tile
				mIsHighlighting = true;
				mHighlightLoc = i.second->getPos();
				mHighlightTile = i.second;
				return 0;
			}
		}
	}

	// If no tile was selected
	mIsHighlighting = false;
	mHighlightTile = nullptr;
}

// Checks if grid is completely filled out and correct
bool GridManager::checkWinState()
{
	if (mCompletedRows.size() == mGridSize && mCompletedColumns.size() == mGridSize && mCompletedBoxes.size() == mGridSize)
	{
		return true;
	}
	return false;
}

// Checks if a specific row was completed
void GridManager::updateRowState(int y)
{
	// Check through row
	int amountCleared = 0;
	for (int i = 0; i < mGridSize; i++)
	{
		// Checks if current tile has a correct input it
		if (mGridMap[y * mGridSize + i]->getValue() != 0 && !mGridMap[y * mGridSize + i]->getIsWrong())
		{
			amountCleared++;
		}
	}

	// If the number of correct tiles equals grid size, the row was completed
	if (amountCleared == mGridSize)
	{
		mCompletedRows.push_back(y);
	}
}

// Checks if a specific column was completed
void GridManager::updateColumnState(int x)
{
	// Check through column
	int amountCleared = 0;
	for (int i = 0; i < mGridSize; i++)
	{
		// Checks if current tile has a correct input it
		if (mGridMap[i * mGridSize + x]->getValue() != 0 && !mGridMap[i * mGridSize + x]->getIsWrong())
		{
			amountCleared++;
		}
	}

	// If the number of correct tiles equals grid size, the column was completed
	if (amountCleared == mGridSize)
	{
		mCompletedColumns.push_back(x);
	}
}

// Checks if a specific box was completed
void GridManager::updateBoxState(int x, int y)
{
	// Check through box
	int amountCleared = 0;
	for (int i = y - (y % mBoxSizeY); i <= (y - (y % mBoxSizeY)) + (mBoxSizeY - 1); i++)
	{
		for (int j = x - (x % mBoxSizeX); j <= (x - (x % mBoxSizeX)) + (mBoxSizeX - 1); j++)
		{
			// Checks if current tile has a correct input it
			if (mGridMap[i * mGridSize + j]->getValue() && !mGridMap[i * mGridSize + j]->getIsWrong())
			{
				amountCleared++;
			}
		}
	}

	// If the number of correct tiles equals grid size, the box was completed
	if (amountCleared == mGridSize)
	{
		mCompletedBoxes.push_back((x / mBoxSizeX) + ((y % mBoxSizeY) * mBoxSizeY));
	}
}

// Changes the current selected tile's value
void GridManager::changeValue(int value)
{
	EventSystem* pEventSystem = EventSystem::getInstance();

	// Only changes value if a tile is currently selected that isn't a default tile
	if (mIsHighlighting && !mHighlightTile->getDefault())
	{
		Vector2D pos = mHighlightTile->getGridPos();

		// Only changes tile value if current value is not already correct
		if (mHighlightTile->getValue() != mGridFiller->getValue(pos.getX(), pos.getY()))
		{
			// If the number is wrong change the font color of that number
			if (value != mGridFiller->getValue(pos.getX(), pos.getY()) && value != 0)
			{
				mHighlightTile->setIsWrong(true);
				mHighlightTile->changeFontColor(mWrongInputColor);

				// Detract from score
				GameEvent gameEvent(MINUS_SCORE_EVENT);
				pEventSystem->fireEvent(gameEvent);
			}
			else
			{
				mHighlightTile->setIsWrong(false);
				mHighlightTile->changeFontColor(Color()); // Changes back to original number color
			}

			// Sets tile equal to value unless it is already equal, then set to 0
			if (value != mHighlightTile->getValue())
			{
				mHighlightTile->changeValue(value);
				removeNotes();

				// Checks if value is correct
				if (value == mGridFiller->getValue(pos.getX(), pos.getY()) && value != 0)
				{
					// Update current board state and add score
					updateRowState(pos.getY());
					updateColumnState(pos.getX());
					updateBoxState(pos.getX(), pos.getY());
					GameEvent gameEvent(ADD_SCORE_EVENT);
					pEventSystem->fireEvent(gameEvent);

					// Check if game is won
					if (checkWinState())
					{
						GameEvent gameEvent(WIN_GAME_EVENT);
						pEventSystem->fireEvent(gameEvent);
					}
				}
			}
			else
				mHighlightTile->changeValue(0);
		}
	}
}

// Removes notes in the selected tile's box/column/row that share the same value as that tile
void GridManager::removeNotes()
{
	int num = mHighlightTile->getValue();
	Vector2D pos = mHighlightTile->getGridPos();
	int x = pos.getX();
	int y = pos.getY();

	// Check through row
	for (int i = 0; i < mGridSize; i++)
	{
		// Turns off note for the number
		if (mGridMap[y * mGridSize + i]->getNoteValue(num))
		{
			mGridMap[y * mGridSize + i]->turnOnOffNote(num);
		}
	}

	// Check through column
	for (int i = 0; i < mGridSize; i++)
	{
		// Turns off note for the number
		if (mGridMap[i * mGridSize + x]->getNoteValue(num))
		{
			mGridMap[i * mGridSize + x]->turnOnOffNote(num);
		}
	}

	// Check through whole box
	for (int i = y - (y % mBoxSizeY); i <= (y - (y % mBoxSizeY)) + (mBoxSizeY - 1); i++)
	{
		for (int j = x - (x % mBoxSizeX); j <= (x - (x % mBoxSizeX)) + (mBoxSizeX - 1); j++)
		{
			// Turns off note for the number
			if (mGridMap[i * mGridSize + j]->getNoteValue(num))
			{
				mGridMap[i * mGridSize + j]->turnOnOffNote(num);
			}
		}
	}
}

// Check if given row is completed
bool GridManager::isRowCompleted(int y)
{
	auto it = find(mCompletedRows.begin(), mCompletedRows.end(), y);
	if (it != mCompletedRows.end())
		return true;

	return false;
}

// Check if given column is completed
bool  GridManager::isColumnCompleted(int x)
{
	auto it = find(mCompletedColumns.begin(), mCompletedColumns.end(), x);
	if (it != mCompletedColumns.end())
		return true;

	return false;
}

// Check if box column is completed
bool  GridManager::isBoxCompleted(int x, int y)
{
	auto it = find(mCompletedBoxes.begin(), mCompletedBoxes.end(), (x / mBoxSizeX) + ((y % mBoxSizeY) * mBoxSizeY));
	if (it != mCompletedBoxes.end())
		return true;
	return false;
}