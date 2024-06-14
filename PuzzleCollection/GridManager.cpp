#include "GridManager.h"

GridManager::GridManager(GraphicsSystem& graphicsSystem, GridFiller& gridFiller)
{
	mGraphicsSystem = &graphicsSystem;
	mGridFiller = &gridFiller;
}

GridManager::~GridManager()
{
	clearGrid();
}

void GridManager::init(GraphicsBufferManager& graphicsBufferManager, int tileIndex)
{
	mGraphicsBufferManager = &graphicsBufferManager;
	mTileSize = mGraphicsBufferManager->getBuffer(tileIndex)->getSize().getX();
}

void GridManager::loadGridVariables(int gridSize, int boxSizeX, int boxSizeY, int percentRemoved, float tilePadding, float notePadding)
{
	mGridSize = gridSize;
	mBoxSizeX = boxSizeX;
	mBoxSizeY = boxSizeY;
	mPercentRemoved = percentRemoved;
	mTilePadding = tilePadding;
	mNotePadding = notePadding;
}

void GridManager::loadColorData(Color& default, Color& playerInput, Color& sameNumber, Color& wrong)
{
	mDefaultNumberColor = default;
	mPlayerInputColor = playerInput;
	mSameNumberColor = sameNumber;
	mWrongInputColor = wrong;
}

void GridManager::loadFontData(string assetPath, string fontName, int numberFontSize, int noteFontSize)
{
	mAssetPath = assetPath;
	mFontName = fontName;
	mNumberFontSize = numberFontSize;
	mNoteFontSize = noteFontSize;
}

void GridManager::loadGrid(GridType gridType, int dispWidth, int dispHeight)
{
	mCurrentGrid = gridType;

	// Centers the grid
	float startPosX = (dispWidth / 2) - ((mGridSize * (mTileSize + mTilePadding)) / 2);
	float startPosY = (dispHeight / 2) - ((mGridSize * (mTileSize + mTilePadding)) / 2);

	// Set up grid according to current game
	if (mCurrentGrid == SUDOKU)
	{
		mGridFiller->fillGrid();
		for (int i = 0; i < mGridSize; i++)
		{
			for (int j = 0; j < mGridSize; j++)
			{
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

void GridManager::clearGrid()
{
	for (auto& i : mGridMap)
	{
		delete i.second;
		i.second = nullptr;
	}
	mGridMap.clear();
}

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
		// Checks what color the tile needs to be
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

int GridManager::checkInput(Vector2D loc)
{
	float x = loc.getX();
	float y = loc.getY();

	for (auto& i : mGridMap)
	{
		Vector2D tileLoc = i.second->getPos();
		// Checks if mouse click is in bounds of a tile
		if (x >= tileLoc.getX() && x <= tileLoc.getX() + mTileSize)
		{
			if (y >= tileLoc.getY() && y <= tileLoc.getY() + mTileSize)
			{
				mIsHighlighting = true;
				mHighlightLoc = i.second->getPos();
				mHighlightTile = i.second;
				return 0;
			}
		}
	}
	mIsHighlighting = false;
	mHighlightTile = nullptr;
}

void GridManager::changeValue(int value)
{
	EventSystem* pEventSystem = EventSystem::getInstance();

	if (mIsHighlighting && !mHighlightTile->getDefault())
	{
		// Checks if current value is already correct
		Vector2D pos = mHighlightTile->getGridPos();
		if (mHighlightTile->getValue() != mGridFiller->getValue(pos.getX(), pos.getY()))
		{
			// If the number is wrong change the font color of that number to white
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
				mHighlightTile->changeFontColor(Color());
			}

			// Sets tile equal to value unless it is already equal, then set to 0
			if (value != mHighlightTile->getValue())
			{
				mHighlightTile->changeValue(value);
				removeNotes();

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

void GridManager::updateRowState(int y)
{
	// Check through row
	int amountCleared = 0;
	for (int i = 0; i < mGridSize; i++)
	{
		if (mGridMap[y * mGridSize + i]->getValue() != 0 && !mGridMap[y * mGridSize + i]->getIsWrong())
		{
			amountCleared++;
		}
	}
	if (amountCleared == mGridSize)
	{
		mCompletedRows.push_back(y);
	}
}

void GridManager::updateColumnState(int x)
{
	// Check through column
	int amountCleared = 0;
	for (int i = 0; i < mGridSize; i++)
	{
		if (mGridMap[i * mGridSize + x]->getValue() != 0 && !mGridMap[i * mGridSize + x]->getIsWrong())
		{
			amountCleared++;
		}
	}
	if (amountCleared == mGridSize)
	{
		mCompletedColumns.push_back(x);
	}
}

void GridManager::updateBoxState(int x, int y)
{
	// Check through box
	int amountCleared = 0;
	for (int i = y - (y % mBoxSizeY); i <= (y - (y % mBoxSizeY)) + (mBoxSizeY - 1); i++)
	{
		for (int j = x - (x % mBoxSizeX); j <= (x - (x % mBoxSizeX)) + (mBoxSizeX - 1); j++)
		{
			if (mGridMap[i * mGridSize + j]->getValue() && !mGridMap[i * mGridSize + j]->getIsWrong())
			{
				amountCleared++;
			}
		}
	}
	if (amountCleared == mGridSize)
	{
		mCompletedBoxes.push_back((x / mBoxSizeX) + ((y % mBoxSizeY) * mBoxSizeY));
	}
}

bool GridManager::checkWinState()
{
	/*
	cout << "Rows: " << mCompletedRows.size() << endl;
	cout << "Columns: " << mCompletedColumns.size() << endl;
	cout << "Boxes: " << mCompletedBoxes.size() << endl;
	cout << "\n";*/
	if (mCompletedRows.size() == mGridSize && mCompletedColumns.size() == mGridSize && mCompletedBoxes.size() == mGridSize)
	{
		return true;
	}
	return false;
}

void GridManager::removeNotes()
{
	int num = mHighlightTile->getValue();
	Vector2D pos = mHighlightTile->getGridPos();
	int x = pos.getX();
	int y = pos.getY();

	// Check through row
	for (int i = 0; i < mGridSize; i++)
	{
		if (mGridMap[y * mGridSize + i]->getNoteValue(num))
		{
			mGridMap[y * mGridSize + i]->turnOnOffNote(num);
		}
	}

	// Check through column
	for (int i = 0; i < mGridSize; i++)
	{
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