#pragma once
#include <Trackable.h>
#include <Vector2D.h>
#include <vector>
#include <iostream>

using namespace std;

class Tile : public Trackable
{
public:
	Tile(int width, int height);
	~Tile();

	Vector2D getSize() { return Vector2D(mWidth, mHeight); }
private:
	int mWidth;
	int mHeight;
	int mValue; // Actual number in the tile; 0 = no number
	vector<int> mNotes; // Notes the player assigns the tile for what possible numbers it can be
};