#include "Trackable.h"
#include <vector>
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
	void fillLine(int start, int end, int row);
	int getAvailableNum(int x, int y);

	vector<int> checkRow(int x, int y);
	vector<int> checkColumn(int x, int y);
	vector<int> checkBox(int x, int y);

	vector<int> combineVectors(vector<int> a, vector<int> b);
private:
	int mSize, mBoxSizeX, mBoxSizeY; // column and row size is always equal while the box sizes can be different but must multiply together to equal size
	vector<int> nums;
	vector<vector<int>> grid;
};