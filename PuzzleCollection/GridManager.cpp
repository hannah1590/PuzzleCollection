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

void GridManager::loadGrid(GridType gridType, int gridSize, int dispWidth, int dispHeight)
{
	mCurrentGrid = gridType;
	mGridSize = gridSize;

	// Centers the grid
	float startPosX = (dispWidth / 2) - ((mGridSize * (mTileSize + PADDING)) / 2);
	float startPosY = (dispHeight / 2) - ((mGridSize * (mTileSize + PADDING)) / 2);

	// Set up grid according to current game
	if (mCurrentGrid == SUDOKU)
	{
		mGridFiller->fillGrid();
		for (int i = 0; i < mGridSize; i++)
		{
			for (int j = 0; j < mGridSize; j++)
			{
				mGridMap[i * mGridSize + j] = new Tile(Vector2D(j * (mTileSize + PADDING) + startPosX, i * (mTileSize + PADDING) + startPosY), mGridSize);
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

void GridManager::draw(int boxSizeX, int boxSizeY, int xSeparatorIndex, int ySeparatorIndex, int tileIndex, int xHighlightIndex, int yHighlightIndex)
{
	Vector2D loc;
	// Draw lines on the x-axis between each box
	for (int i = 0; i < mGridSize / boxSizeY; i++)
	{
		loc = mGridMap[(i * (mGridSize / boxSizeY)) * mGridSize]->getPos() - Vector2D(PADDING, PADDING);
		mGraphicsSystem->drawBackbuffer(loc, *mGraphicsBufferManager->getBuffer(xSeparatorIndex), 1.0f);
	}
	loc = mGridMap[(mGridSize - 1) * mGridSize]->getPos() + Vector2D(-PADDING, (float)mTileSize);
	mGraphicsSystem->drawBackbuffer(loc, *mGraphicsBufferManager->getBuffer(xSeparatorIndex), 1.0f);
	
	// Draw lines on the y-axis between each box
	for (int i = 0; i < mGridSize / boxSizeX; i++)
	{
		loc = mGridMap[i * (mGridSize / boxSizeX)]->getPos() - Vector2D(PADDING, PADDING);
		mGraphicsSystem->drawBackbuffer(loc, *mGraphicsBufferManager->getBuffer(ySeparatorIndex), 1.0f);
	}
	loc = mGridMap[(mGridSize - 1)]->getPos() + Vector2D((float)mTileSize, -PADDING);
	mGraphicsSystem->drawBackbuffer(loc, *mGraphicsBufferManager->getBuffer(ySeparatorIndex), 1.0f);

	Color white(255, 255, 255, 255);
	Font font(ASSET_PATH + FONT_FILENAME, FONT_SIZE);
	Font notesFont(ASSET_PATH + FONT_FILENAME, 15);

	// Draw each tile with the number
	for (int i = 0; i < mGridMap.size(); i++)
	{
		Vector2D loc = mGridMap[i]->getPos();
		mGraphicsSystem->drawBackbuffer(loc, *mGraphicsBufferManager->getBuffer(tileIndex), 1.0f);
		// Draws correct tile value
		//mGraphicsSystem->writeTextToBackbuffer(loc + Vector2D(mTileSize / 2.0f, PADDING), font, white, to_string(mGridFiller->getValue(i % mGridSize, i / mGridSize)), true);
		// Draws player inputed tile value
		if (mGridMap[i]->getValue() == 0)
		{
			for (auto& i : mGridMap[i]->getNotes())
			{
				// TO DO: figure out note spacing
				mGraphicsSystem->writeTextToBackbuffer(loc + Vector2D(i.second % 3 * 5, i.second / 3 * 5), notesFont, white, to_string(i.second + 1), false);
			}
		}
		else
			mGraphicsSystem->writeTextToBackbuffer(loc + Vector2D(mTileSize / 2.0f, PADDING), font, white, to_string(mGridMap[i]->getValue()), true);
	}

	// Draw highlighted box if a number is currently clicked
	if (mIsHighlighting)
	{
		Vector2D loc = mHighlightLoc - Vector2D(PADDING, PADDING);
		mGraphicsSystem->drawBackbuffer(loc, *mGraphicsBufferManager->getBuffer(xHighlightIndex), 1.0f);
		loc = mHighlightLoc + Vector2D(-PADDING, (float)mTileSize);
		mGraphicsSystem->drawBackbuffer(loc, *mGraphicsBufferManager->getBuffer(xHighlightIndex), 1.0f);

		loc = mHighlightLoc - Vector2D(PADDING, PADDING);
		mGraphicsSystem->drawBackbuffer(loc, *mGraphicsBufferManager->getBuffer(yHighlightIndex), 1.0f);
		loc = mHighlightLoc + Vector2D((float)mTileSize, -PADDING);
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
}
