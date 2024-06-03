#pragma once
#include "GraphicsSystem.h"
#include <iostream>
#include <fstream>
#include "Font.h"
#include "Color.h"
#include "Timer.h"

// Done by Nate
// Loads all UI elements of game
class HUD : public Trackable
{
public:
	HUD(GraphicsSystem& graphicsSystem);
	~HUD();

	void update(int score, float savedTime);
	void pauseTimer();
	void reset();
	void loadData(string& filename);

	float getTime() { return mTime; }
private:
	void draw();

	GraphicsSystem* mGraphicsSystem;

	string mScoreText, mTimeText;
	Timer mTimer;
	float mTime;
	int mScore;

	const string ASSET_PATH = "..\\..\\PuzzleCollection\\libraries\\assets\\";
	const string FONT_FILENAME = "cour.ttf";
	const int FONT_SIZE = 50;
};