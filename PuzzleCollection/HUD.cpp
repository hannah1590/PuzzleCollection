#include "HUD.h"

HUD::HUD(GraphicsSystem& graphicsSystem, int dispWidth, int dispHeight)
{
	mGraphicsSystem = &graphicsSystem;
	mDispWidth = dispWidth;
	mDispHeight = dispHeight;
}

HUD::~HUD()
{
}

void HUD::init(GraphicsBufferManager& graphicsBufferManager, int tileIndex, int gridSize)
{
	mGraphicsBufferManager = &graphicsBufferManager;
	mTileIndex = tileIndex;
	mGridSize = gridSize;
	mTileSize = mGraphicsBufferManager->getBuffer(mTileIndex)->getSize().getX() * 1.2f;

	// Start drawing position should have the buttons in a row centered in the middle
	mStartNumLoc = (mDispWidth / 2) - ((mGridSize * (mTileSize + PADDING)) / 2);
	// Note button location is to the right of the numbers
	mNotesButtonLoc = Vector2D(mGridSize * (mTileSize + PADDING) + mStartNumLoc + (mTileSize / 2), mDispHeight - (mTileSize + PADDING));

	// Assign locations to each number tile
	for (int i = 0; i < mGridSize; i++)
	{
		mNumMap[i] = Vector2D(i * (mTileSize + PADDING) + mStartNumLoc, mDispHeight - (mTileSize + PADDING));
	}
}

void HUD::update(float savedTime, bool notes)
{
	mNotesOn = notes;
	mTimer.pause(true);
	mTimer.pause(false);

	mTime = mTimer.getElapsedTime();
	mTime = mTime / 1000;
	mTime += savedTime;

	draw();
}

void HUD::draw()
{
	Color lightGray(220, 220, 220, 255);
	Color blueGray(140, 166, 250, 255);
	Color gray(65, 65, 65, 255);
	Font font(ASSET_PATH + FONT_FILENAME, FONT_SIZE);
	Font noteFont(ASSET_PATH + FONT_FILENAME, NOTE_FONT_SIZE);

	Vector2D timeLoc(500, 0);

	string timeText = mTimeText + ": " + to_string(mTime);

	mGraphicsSystem->writeTextToBackbuffer(timeLoc, font, lightGray, timeText, false);

	// Draw each tile and the numbers on them
	for (auto& i : mNumMap)
	{
		mGraphicsSystem->drawBackbuffer(i.second, *mGraphicsBufferManager->getBuffer(mTileIndex), 1.2f);
		if(!mNotesOn)
			mGraphicsSystem->writeTextToBackbuffer(i.second + Vector2D(mTileSize / 2.0f), font, lightGray, to_string(i.first + 1), true);
		else
			mGraphicsSystem->writeTextToBackbuffer(i.second + Vector2D(mTileSize / 2.0f), font, blueGray, to_string(i.first + 1), true);
	}

	// Draw notes button
	mGraphicsSystem->drawButtons(1, 0, mTileSize * 2, mNotesButtonLoc.getX(), mNotesButtonLoc.getY(), gray);
	mGraphicsSystem->writeTextToBackbuffer(mNotesButtonLoc + Vector2D((mTileSize * 2.0f) / 2.0f, (mTileSize * 2.0f) / 8.0f), noteFont, lightGray, "Notes", true);
}

int HUD::checkInput(Vector2D loc)
{
	EventSystem* pEventSystem = EventSystem::getInstance();

	float x = loc.getX();
	float y = loc.getY();

	if (x >= mNotesButtonLoc.getX() && x <= mNotesButtonLoc.getX() + mTileSize * 2)
	{
		if (y >= mNotesButtonLoc.getY() && y <= mDispWidth)
		{
			GameEvent gameEvent(OPEN_CLOSE_NOTES_EVENT);
			pEventSystem->fireEvent(gameEvent);
		}
	}
	else
	{
		for (auto& i : mNumMap)
		{
			if (x >= i.second.getX() && x <= i.second.getX() + mTileSize)
			{
				if (y >= i.second.getY() && y <= mDispWidth)
				{
					return i.first + 1;
				}
			}
		}
	}
	return 0;
}

void HUD::pauseTimer()
{
	mTimer.pause(true);
}

void HUD::reset()
{
	mTimer.stop();
	mTimer.start();
	mTimer.pause(true);
}

void HUD::loadData(string& filename)
{
	ifstream data(filename);
	string currentString;

	while (!data.eof())
	{
		data >> currentString;
		if (currentString == "HUD")
		{
			data >> mTimeText;
		}
	}
}