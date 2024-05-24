#include "GridManager.h";
GridManager::GridManager(int size, int boxX, int boxY)
{
    srand(time(NULL));

    mSize = size;
    mBoxSizeX = boxX;
    mBoxSizeY = boxY;

    initGrid();
    for (int i = 0; i < mSize; i++)
    {
        fillLine(0, mSize - 1, i);
    }
    printGrid();
}

GridManager::~GridManager()
{
}

void GridManager::initGrid()
{
    for (int i = 0; i < mSize; i++)
    {
        nums.push_back(i + 1); // Fills the nums vector with all available number options
        std::vector<int> temp;
        for (int j = 0; j < mSize; j++)
        {
            temp.push_back(0);
        }
        grid.push_back(temp);
    }
}

// Works half the time, but will often throw a stack overflow error; figure out a way to optimize it with less brute force and more reliabillity
void GridManager::fillLine(int start, int end, int row)
{
    for (int i = start; i <= end; i++)
    {
        grid[i][row] = getAvailableNum(i, row);
        if (grid[i][row] == 0 && i != 0)
            fillLine(0, i, row);
        else if (grid[i][row] == 0 && i == 0)
        {
            fillLine(0, mSize - 1, row - 1);
            fillLine(0, i, row);
        }
            
    }
}

/*
(0,8) (1,8) (2,8) | (3,8) (4,8) (5,8) | (6,8) (7,8) (8,8)
(0,7) (1,7) (2,7) | (3,7) (4,7) (5,7) | (6,7) (7,7) (8,7)
(0,6) (1,6) (2,6) | (3,6) (4,6) (5,6) | (6,6) (7,6) (8,6)
---------------------------------------------------------
(0,5) (1,5) (2,5) | (3,5) (4,5) (5,5) | (6,5) (7,5) (8,5)
(0,4) (1,4) (2,4) | (3,4) (4,4) (5,4) | (6,4) (7,4) (8,4)
(0,3) (1,3) (2,3) | (3,3) (4,3) (5,3) | (6,3) (7,3) (8,3)
---------------------------------------------------------
(0,2) (1,2) (2,2) | (3,2) (4,2) (5,2) | (6,2) (7,2) (8,2)
(0,1) (1,1) (2,1) | (3,1) (4,1) (5,1) | (6,1) (7,1) (8,1)
(0,0) (1,0) (2,0) | (3,0) (4,0) (5,0) | (6,0) (7,0) (8,0)

(0,5) (1,5) (2,5) | (3,5) (4,5) (5,5) 
(0,4) (1,4) (2,4) | (3,4) (4,4) (5,4) 
-------------------------------------
(0,3) (1,3) (2,3) | (3,3) (4,3) (5,3)
(0,2) (1,2) (2,2) | (3,2) (4,2) (5,2)
-------------------------------------
(0,1) (1,1) (2,1) | (3,1) (4,1) (5,1)
(0,0) (1,0) (2,0) | (3,0) (4,0) (5,0)
*/

// Currently for Sudoku
// Doesn't work yet, can't completely randomize a sudoku problem as the computer will get stuck eventually
void GridManager::fillGrid()
{
    for (int y = 0; y < mSize; y++)
    {
        for (int x = 0; x < mSize; x++)
        {
            grid[x][y] = getAvailableNum(x, y);
        }
    }
}

// Gets a randomly generated number that doesn't yet appear in the row/column/box; if one cannot be found returns 0
int GridManager::getAvailableNum(int x, int y)
{
    std::vector<int> numAvailable = nums;

    // Check columns for repeating numbers
    int temp = 1;
    while (x - temp >= 0)
    {
        // Finds location of the number
        auto it = std::find(numAvailable.begin(), numAvailable.end(),
            grid[x - temp][y]);

        // Remove number from available
        if (it != numAvailable.end()) {
            numAvailable.erase(it);
        }
        temp++;
    }

    // Check rows for repeating numbers
    temp = 1;
    while (y - temp >= 0)
    {
        // Finds location of the number
        auto it = std::find(numAvailable.begin(), numAvailable.end(),
            grid[x][y - temp]);

        // Remove number from available
        if (it != numAvailable.end()) {
            numAvailable.erase(it);
        }
        temp++;
    }
    // Checks box for repeats
    for (int j = x % mBoxSizeX; j >= 0; j--) // Finds which column in box x is at and goes left until it hits next box/end for row y
    {
        // Finds location of the number
        auto it = std::find(numAvailable.begin(), numAvailable.end(),
            grid[x - j][y]);

        // Remove number from available
        if (it != numAvailable.end()) {
            numAvailable.erase(it);
        }
    }
    for (int i = y % mBoxSizeY; i > 0; i--) // Starts at the beginning row of the current box and goes up to the current row
    {
        for (int j = x - (x % mBoxSizeX); j <= (x - (x % mBoxSizeX)) + (mBoxSizeX - 1); j++) // Go through each column in the rows beneath the current row
        {
            // Finds location of the number
            auto it = std::find(numAvailable.begin(), numAvailable.end(),
                grid[j][y - i]);

            // Remove number from available
            if (it != numAvailable.end()) {
                numAvailable.erase(it);
            }
        }
    }
    if (numAvailable.size() != 0)
        return numAvailable[rand() % numAvailable.size()];
    else
        return 0;
}

void GridManager::printGrid()
{
    for (int i = 0; i < mSize; i++)
    {
        for (int j = 0; j < mSize; j++)
        {
            std::cout << grid[j][i] << " ";
            if (j % mBoxSizeX == mBoxSizeX - 1)
            {
                std::cout << "|";
            }
        }
        std::cout << std::endl;
        if (i % mBoxSizeY == mBoxSizeY - 1)
        {
            std::cout << "----------------------------------" << std::endl;
        }
    }

    std::cout << "\n\n";
}