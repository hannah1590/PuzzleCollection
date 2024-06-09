#include "Tile.h"

Tile::Tile(Vector2D pos, Vector2D gridPos, int gridSize)
{
	mPosition = pos;
	mGridPos = gridPos;
	mValue = 0;
	mDefault = false;

	for (int i = 0; i < gridSize; i++)
	{
		mNotes[i] = false;
	}	

	mCurrentColor = mPlayerInputColor;
	mOriginalColor = mCurrentColor;
}

Tile::~Tile()
{
	mNotes.clear();
}

void Tile::init(GraphicsSystem& graphicsSystem, GraphicsBufferManager& graphicsBufferManager, float tilePadding)
{
	mGraphicsSystem = &graphicsSystem;
	mGraphicsBufferManager = &graphicsBufferManager;
	mTilePadding = tilePadding;
}

void Tile::loadColorData(Color& default, Color& playerInput, Color& sameNumber, Color& wrong)
{
	mDefaultNumberColor = default;
	mPlayerInputColor = playerInput;
	mSameNumberColor = sameNumber;
	mWrongNumberColor = wrong;
}

void Tile::loadFontData(Font& numberFont, Font& notesFont)
{
	mNumberFont = numberFont;
	mNotesFont = notesFont;
}

void Tile::setDefault()
{
	mDefault = true;
	mCurrentColor = mDefaultNumberColor;
	mOriginalColor = mCurrentColor;
}

// Swap value of note
void Tile::turnOnOffNote(int value)
{
	mNotes[value - 1] = !mNotes[value - 1];
}

void Tile::changeFontColor(Color& color)
{
	if (!mIsWrong)
		mCurrentColor = color;
}

void Tile::draw(int bufferIndex)
{
	GraphicsBuffer* graphicsBuffer = mGraphicsBufferManager->getBuffer(bufferIndex);

	// Draws tile
	mGraphicsSystem->drawBackbuffer(mPosition, *graphicsBuffer, 1.0f);

	// Draws player inputed tile value, unless value equals 0 then draw the active notes
	if (mValue == 0)
	{
		for (auto& i : mNotes)
		{
			// Check if note is activated
			if (i.second)
			{
				// Draws note inside the tile
				Vector2D noteLoc = mPosition + Vector2D(i.first % 3 * NOTE_SPACING + mTilePadding, i.first / 3 * NOTE_SPACING);
				mGraphicsSystem->writeTextToBackbuffer(noteLoc, mNotesFont, mDefaultNumberColor, to_string(i.first + 1), true);
			}
		}
	}
	else
	{
		mGraphicsSystem->writeTextToBackbuffer(mPosition + Vector2D(graphicsBuffer->getSize().getX() / 2.0f, mTilePadding), mNumberFont, mCurrentColor, to_string(mValue), true);
	}
}