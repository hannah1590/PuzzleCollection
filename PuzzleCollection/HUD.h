#pragma once
#include "GraphicsSystem.h"
#include "Font.h"
#include "Color.h"

class HUD : public Trackable
{
public:
	HUD(GraphicsSystem& graphicsSystem);
	~HUD();

	void welcomeScreen();
	void pauseScreen();
	void gameOver();

	void score(int score);
	void fps(float fps);
	void timer(int timer);
private:
	GraphicsSystem* mGraphicsSystem;

	const string ASSET_PATH = "..\\..\\PuzzleCollection\\libraries\\assets\\";
	const string FONT_FILENAME = "cour.ttf";
	const int FONT_SIZE = 50;

	const int DISP_WIDTH = 800;
	const int DISP_HEIGHT = 600;
};