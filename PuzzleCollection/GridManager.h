#include "Trackable.h"
#include <vector>
#include <random>

class GridManager : public Trackable
{
public:
	GridManager(int size, int subX, int subY);
	~GridManager();

	void initGrid();
	void fillGrid();
	void printGrid();
private:
	int mSize, mSubSquareX, mSubSquareY; // column and row size is always equal while the sub square sizes can be different but must multiply together to equal size
	std::vector<int> nums;
	std::vector<std::vector<int>> grid;
};