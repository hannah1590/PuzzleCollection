#pragma once
#include <Trackable.h>
#include <Vector2D.h>
#include <map>
#include <iostream>

using namespace std;

class Tile : public Trackable
{
public:
	Tile(Vector2D pos, int gridSize);
	~Tile();

	Vector2D getPos() { return mPosition; }
	int getValue() { return mValue; }
	map<int, bool> getNotes() { return mNotes; }
	void changeValue(int value) { mValue = value; }
	void addNote(int value) { mNotes[value - 1] = true; }
	void removeNote(int value){ mNotes[value - 1] = false; }
private:
	Vector2D mPosition;
	int mValue; // Actual number in the tile; 0 = no number
	map<int, bool> mNotes; // Notes the player assigns the tile for what possible numbers it can be
};