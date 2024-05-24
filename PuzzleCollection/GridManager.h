#include "Trackable.h"
#include <vector>
#include <random>

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
private:
	int mSize, mBoxSizeX, mBoxSizeY; // column and row size is always equal while the box sizes can be different but must multiply together to equal size
	std::vector<int> nums;
	std::vector<std::vector<int>> grid;
};