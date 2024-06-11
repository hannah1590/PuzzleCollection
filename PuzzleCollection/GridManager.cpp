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
	if (mIsHighlighting && !mHighlightTile->getDefault())
	{
		// If the number is wrong change the font color of that number to white
		Vector2D pos = mHighlightTile->getGridPos();
		if (value != mGridFiller->getValue(pos.getX(), pos.getY()) && value != 0)
		{
			mHighlightTile->setIsWrong(true);
			mHighlightTile->changeFontColor(mWrongInputColor);
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
		}
		else
			mHighlightTile->changeValue(0);
	}
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
