#include "HUD.h"

HUD::HUD(GraphicsSystem& graphicsSystem)
{
	mGraphicsSystem = &graphicsSystem;
	//mScoreText = "Score";
	//mTimeText = "Time";
}

HUD::~HUD()
{
}

void HUD::update(int score, float savedTime)
{
	mTimer.pause(true);
	mTimer.pause(false);

	mTime = mTimer.getElapsedTime();
	mTime = mTime / 1000;
	mTime += savedTime;

	mScore = score;
	draw();
}

void HUD::draw()
{
	Color white(255, 255, 255, 255);
	Font font(ASSET_PATH + FONT_FILENAME, FONT_SIZE);

	Vector2D scoreLoc(0, 0);
	Vector2D timeLoc(500, 0);

	string scoreText = mScoreText + ": " + to_string(mScore);
	string timeText = mTimeText + ": " + to_string(mTime);

	mGraphicsSystem->writeTextToBackbuffer(scoreLoc, font, white, scoreText);
	mGraphicsSystem->writeTextToBackbuffer(timeLoc, font, white, timeText);
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
			data >>
				mScoreText >>
				mTimeText;
		}
	}
}