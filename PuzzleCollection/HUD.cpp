#include "HUD.h"

HUD::HUD(GraphicsSystem& graphicsSystem)
{
	mGraphicsSystem = &graphicsSystem;
}

HUD::~HUD()
{
}

// TO-DO: allow UI screen variables to be adjusted in a text file

// Displays welcome screen
void HUD::welcomeScreen()
{
	Vector2D loc1(300, 200);
	Vector2D loc2(125, 300);
	string text1 = "Welcome!";
	string text2 = "Press Space to Start";
	Color white(255, 255, 255, 255);
	Font font(ASSET_PATH + FONT_FILENAME, FONT_SIZE);

	mGraphicsSystem->writeTextToBackbuffer(loc1, font, white, text1);
	mGraphicsSystem->writeTextToBackbuffer(loc2, font, white, text2);
}

// Displays pause screen
void HUD::pauseScreen()
{
	Vector2D loc1(300, 200);
	Vector2D loc2(100, 300);
	Vector2D loc3(100, 400);
	string text1 = "Paused";
	string text2 = "Press Space to Resume";
	string text3 = " and Escape to Exit";
	Color white(255, 255, 255, 255);
	Font font(ASSET_PATH + FONT_FILENAME, FONT_SIZE);

	mGraphicsSystem->writeTextToBackbuffer(loc1, font, white, text1);
	mGraphicsSystem->writeTextToBackbuffer(loc2, font, white, text2);
	mGraphicsSystem->writeTextToBackbuffer(loc3, font, white, text3);
}

// Displays game over screen
void HUD::gameOver()
{
	Vector2D loc1(300, 200);
	Vector2D loc2(125, 300);
	string text1 = "Game Over!";
	string text2 = "Press Escape to Exit";
	Color white(255, 255, 255, 255);
	Font font(ASSET_PATH + FONT_FILENAME, FONT_SIZE);

	mGraphicsSystem->writeTextToBackbuffer(loc1, font, white, text1);
	mGraphicsSystem->writeTextToBackbuffer(loc2, font, white, text2);
}

// Displays score UI
void HUD::score(int score)
{
	Vector2D loc(0, 0);
	string text = "Score: " + to_string(score);
	Color white(255, 255, 255, 255);
	Font font(ASSET_PATH + FONT_FILENAME, FONT_SIZE);

	mGraphicsSystem->writeTextToBackbuffer(loc, font, white, text);
}

// Displays fps
void HUD::fps(float fps)
{
	Vector2D loc(500, 0);
	string text = to_string(fps);
	Color white(255, 255, 255, 255);
	Font font(ASSET_PATH + FONT_FILENAME, FONT_SIZE);

	mGraphicsSystem->writeTextToBackbuffer(loc, font, white, text);
}

// Displays timer UI
void HUD::timer(int timer)
{
	Vector2D loc(500, 500);
	string text = "Time: " + to_string(timer);
	Color white(255, 255, 255, 255);
	Font font(ASSET_PATH + FONT_FILENAME, FONT_SIZE);

	mGraphicsSystem->writeTextToBackbuffer(loc, font, white, text);
}