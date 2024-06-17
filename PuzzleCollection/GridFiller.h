#pragma once
#include <Trackable.h>
#include <Vector2D.h>
#include <vector>
#include <stack>
#include <random>
using namespace std;

/* Creates and randomly fills up a sudoku grid
   by Hannah Fasco
*/ 

class GridFiller : public Trackable
{
public:
	GridFiller() = default;
	~GridFiller();

	void initGrid(int size, int boxX, int boxY);
	void clearGrid();

	void fillGrid();

	vector<int> checkRow(int x, int y);
	vector<int> checkColumn(int x, int y);
	vector<int> checkBox(int x, int y);

	vector<int> combineVectors(vector<int> a, vector<int> b);
	Vector2D findRepeatInColumn(int x, int y, int num);
	Vector2D findRepeatInRow(int x, int y, int num);

	int getValue(int x, int y) { return mGrid[y][x]; }
private:
	void printGrid();

	int mSize, mBoxSizeX, mBoxSizeY;     // Size equals the amount of units on the side of the grid (9x9 = 9) while the box sizes are the amount of units on each axis that make up the box
	vector<int> mNums;                   // All possible numbers that can be in the grid
	vector<vector<int>> mGrid;           // Double vector that contains the grid; access grid values throguh [y][x]
};