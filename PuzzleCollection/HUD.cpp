#include "HUD.h"

// Loads important HUD variables
HUD::HUD(GraphicsSystem& graphicsSystem, int dispWidth, int dispHeight)
{
	mGraphicsSystem = &graphicsSystem;
	mDispWidth = dispWidth;
	mDispHeight = dispHeight;
}

// Deletes current HUD
HUD::~HUD()
{
	reset();
}

// Initializes grid-based variables
void HUD::init(GraphicsBufferManager& graphicsBufferManager, int tileIndex, int gridSize, float tilePadding)
{
	mGraphicsBufferManager = &graphicsBufferManager;
	mTileIndex = tileIndex;
	mGridSize = gridSize;
	mTileSize = mGraphicsBufferManager->getBuffer(mTileIndex)->getSize().getX() * 1.2f;

	// Start drawing position should have the buttons in a row centered in the middle
	mStartNumLoc = (mDispWidth / 2) - ((mGridSize * (mTileSize + tilePadding)) / 2);

	// Note button location is to the right of the numbers
	mNotesButtonLoc = Vector2D(mGridSize * (mTileSize + tilePadding) + mStartNumLoc + (mTileSize / 2), mDispHeight - (mTileSize + tilePadding));

	// Assign locations to each number tile
	for (int i = 0; i < mGridSize; i++)
	{
		mNumMap[i] = Vector2D(i * (mTileSize + tilePadding) + mStartNumLoc, mDispHeight - (mTileSize + tilePadding));
	}
}

// Loads all color data needed
void HUD::loadColorData(Color& text, Color& tile, Color& note)
{
	mTextColor = text;
	mTileColor = tile;
	mNoteUIColor = note;
}

// Loads all font data needed
void HUD::loadFontData(string assetPath, string fontName, int menuFontSize, int numberFontSize)
{
	mAssetPath = assetPath;
	mFontName = fontName;
	mMenuFontSize = menuFontSize;
	mNumberFontSize = numberFontSize;
}

// Loads HUD display data
void HUD::loadData(string& filename)
{
	ifstream data(filename);
	string currentString;

	while (!data.eof())
	{
		data >> currentString;
		if (currentString == "HUD")
		{
			data >> mTimeText >> mScoreText >> mNotesText;
		}
	}
}

// Updates current HUD
void HUD::update(float savedTime, bool notes)
{
	mNotesOn = notes;

	mTimer.pause(true);
	mTimer.pause(false);
	
	// Adjusts HUD time
	mTime = mTimer.getElapsedTime();
	mTime = mTime / 1000;
	mTime += savedTime;

	draw();
}

// Resets timer
void HUD::reset()
{
	mTimer.stop();
	mTimer.start();
	mTimer.pause(true);

	mNumMap.clear();
}

// Checks if the user clicked on a portion of the HUD
int HUD::checkInput(Vector2D loc)
{
	EventSystem* pEventSystem = EventSystem::getInstance();

	float x = loc.getX();
	float y = loc.getY();

	// Check if cursor is in the bounds of the notes button
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
		// Check if cursor is in the bounds of a number tile
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

// Draws current HUD
void HUD::draw()
{
	// Font variables
	Font menuFont(mAssetPath + mFontName, mMenuFontSize);
	Font noteFont(mAssetPath + mFontName, mNumberFontSize);

	// Draw score
	Vector2D scoreLoc(0, 0);
	string scoreText = mScoreText + " " + to_string(mScore);
	mGraphicsSystem->writeTextToBackbuffer(scoreLoc, menuFont, mTextColor, scoreText, false);

	// Draw time
	Vector2D timeLoc(500, 0);
	string timeText;
	int minutes = (int)mTime / 60;
	int seconds = (int)mTime - (minutes * 60);
	if(seconds >= 10)
		timeText = mTimeText + " " + to_string(minutes) + ":" + to_string(seconds);
	else
		timeText = mTimeText + " " + to_string(minutes) + ":0" + to_string(seconds);
	mGraphicsSystem->writeTextToBackbuffer(timeLoc, menuFont, mTextColor, timeText, false);

	// Draw each tile and the numbers on them
	for (auto& i : mNumMap)
	{
		mGraphicsSystem->drawBackbuffer(i.second, *mGraphicsBufferManager->getBuffer(mTileIndex), 1.2f);
		if(!mNotesOn)
			mGraphicsSystem->writeTextToBackbuffer(i.second + Vector2D(mTileSize / 2.0f), menuFont, mTextColor, to_string(i.first + 1), true);
		else
			mGraphicsSystem->writeTextToBackbuffer(i.second + Vector2D(mTileSize / 2.0f), menuFont, mNoteUIColor, to_string(i.first + 1), true);
	}

	// Draw notes button
	mGraphicsSystem->drawButtons(1, 0, mTileSize * 2, mNotesButtonLoc.getX(), mNotesButtonLoc.getY(), mTileColor);
	mGraphicsSystem->writeTextToBackbuffer(mNotesButtonLoc + Vector2D((mTileSize * 2.0f) / 2.0f, (mTileSize * 2.0f) / 8.0f), noteFont, mTextColor, mNotesText, true);
}