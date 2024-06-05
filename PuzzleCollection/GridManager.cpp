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

void GridManager::init(GraphicsBufferManager& graphicsBufferManager, GraphicsBuffer& tileBuffer)
{
	mTileBuffer = &tileBuffer;
	mGraphicsBufferManager = &graphicsBufferManager;
	mTileSize = tileBuffer.getSize().getX();
}

void GridManager::loadGrid(GridType gridType, int gridSize, int dispWidth, int dispHeight)
{
	mCurrentGrid = gridType;
	mGridSize = gridSize;

	// Centers the grid
	float startPosX = (dispWidth / 2) - ((mGridSize * (mTileSize + PADDING)) / 2);
	float startPosY = (dispHeight / 2) - ((mGridSize * (mTileSize + PADDING)) / 2);

	mXSeparatorIndex = mGraphicsBufferManager->getSize(); // Index of next buffer equals size of current buffer array
	mYSeparatorIndex = mXSeparatorIndex++;
	
	// Make buffers for the lines separating each box
	Color white(255, 255, 255, 255);
	mXSeparatorBuffer = new GraphicsBuffer(white, mGridSize * (mTileSize + PADDING) + PADDING, PADDING);
	mYSeparatorBuffer = new GraphicsBuffer(white, PADDING, mGridSize * (mTileSize + PADDING) + PADDING);
	
	// Add buffers to buffer manager
	mGraphicsBufferManager->addBuffer(mXSeparatorIndex, *mXSeparatorBuffer);
	mGraphicsBufferManager->addBuffer(mYSeparatorIndex, *mYSeparatorBuffer);

	// Set up grid according to current game
	if (mCurrentGrid == SUDOKU)
	{
		mGridFiller->fillGrid();
		for (int i = 0; i < mGridSize; i++)
		{
			for (int j = 0; j < mGridSize; j++)
			{
				mGridMap[i * mGridSize + j] = new Tile(j * (mTileSize + PADDING) + startPosX, i * (mTileSize + PADDING) + startPosY);
			}
		}
	}
}

void GridManager::clearGrid()
{
	// Checks if the buffers made in this class currently exist
	if(mGraphicsBufferManager->doesExist(mXSeparatorIndex) && mGraphicsBufferManager->doesExist(mYSeparatorIndex))
	{
		mGraphicsBufferManager->deleteBuffer(mXSeparatorIndex);
		mGraphicsBufferManager->deleteBuffer(mYSeparatorIndex);
	}
	for (auto& i : mGridMap)
	{
		delete i.second;
		i.second = nullptr;
	}
	mGridMap.clear();
}

void GridManager::draw(int boxSizeX, int boxSizeY)
{
	// Draw lines on the x-axis between each box
	for (int i = 0; i < mGridSize / boxSizeY; i++)
	{
		mGraphicsSystem->drawBackbuffer(mGridMap[(i * (mGridSize / boxSizeY)) * mGridSize]->getSize() - Vector2D(PADDING, PADDING), *mXSeparatorBuffer, 1.0f);
	}
	mGraphicsSystem->drawBackbuffer(mGridMap[(mGridSize - 1) * mGridSize]->getSize() + Vector2D(-PADDING, (float)mTileSize), *mXSeparatorBuffer, 1.0f);
	
	// Draw lines on the y-axis between each box
	for (int i = 0; i < mGridSize / boxSizeX; i++)
	{
		mGraphicsSystem->drawBackbuffer(mGridMap[i * (mGridSize / boxSizeX)]->getSize() - Vector2D(PADDING, PADDING), *mYSeparatorBuffer, 1.0f);
	}
	mGraphicsSystem->drawBackbuffer(mGridMap[(mGridSize - 1)]->getSize() + Vector2D((float)mTileSize, -PADDING), *mYSeparatorBuffer, 1.0f);

	Color white(255, 255, 255, 255);
	Font font(ASSET_PATH + FONT_FILENAME, FONT_SIZE);

	// Draw each tile
	for (int i = 0; i < mGridMap.size(); i++)
	{
		Vector2D loc = mGridMap[i]->getSize();
		mGraphicsSystem->drawBackbuffer(loc, *mTileBuffer, 1.0f);
		mGraphicsSystem->writeTextToBackbuffer(loc + Vector2D(PADDING,-PADDING), font, white, to_string(mGridFiller->getValue(i % mGridSize, i / mGridSize)));
	}
}
