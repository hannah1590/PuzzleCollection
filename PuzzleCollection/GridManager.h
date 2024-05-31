#include "Trackable.h"
#include "Vector2D.h"
#include <vector>
#include <stack>
#include <random>

using namespace std;

class GridManager : public Trackable
{
public:
	GridManager(int size, int boxX, int boxY);
	~GridManager();

	void initGrid();
	void fillGrid();
	void printGrid();

	vector<int> checkRow(int x, int y);
	vector<int> checkColumn(int x, int y);
	vector<int> checkBox(int x, int y);

	vector<int> combineVectors(vector<int> a, vector<int> b);
	bool switchNumbers(vector<int> holder, vector<int> options, vector<int> ogAvail, int x, int y, int a, int b);
	Vector2D findNumInBox(int x, int y, int num);
private:
	int mSize, mBoxSizeX, mBoxSizeY; // Column and row size is always equal while the box sizes can be different but must multiply together to equal size
	vector<int> mNums;
	vector<vector<int>> mGrid; // [y][x]
};