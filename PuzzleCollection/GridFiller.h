#pragma once
#include <Trackable.h>
#include <Vector2D.h>
#include <vector>
#include <stack>
#include <random>
using namespace std;

class GridFiller : public Trackable
{
public:
	GridFiller();
	~GridFiller();

	void initGrid(int size, int boxX, int boxY);
	void clearGrid();

	void fillGrid();
	void printGrid(); // for debug purposes

	vector<int> checkRow(int x, int y);
	vector<int> checkColumn(int x, int y);
	vector<int> checkBox(int x, int y);

	vector<int> combineVectors(vector<int> a, vector<int> b);
	Vector2D findRepeatInColumn(int x, int y, int num);
	Vector2D findRepeatInRow(int x, int y, int num);

	int getValue(int x, int y) { return mGrid[y][x]; }
private:
	int mSize, mBoxSizeX, mBoxSizeY; // Column and row size is always equal while the box sizes can be different but must multiply together to equal size
	vector<int> mNums; // All possible numbers that can be in the grid
	vector<vector<int>> mGrid; // [y][x]
};