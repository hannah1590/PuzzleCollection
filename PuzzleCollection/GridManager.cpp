#include "GridManager.h";

GridManager::GridManager(int size, int subX, int subY)
{
    mSize = size;
    mSubSquareX = subX;
    mSubSquareY = subY;

    for (int i = 1; i <= mSize; i++)
    {
        nums.push_back(i);
    }

    initGrid();
    fillGrid();
    printGrid();
}

GridManager::~GridManager()
{
}

void GridManager::initGrid()
{
    for (int i = 0; i < mSize; i++)
    {
        std::vector<int> temp;
        for (int j = 0; j < mSize; j++)
        {
            temp.push_back(0);
        }
        grid.push_back(temp);
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
*/

// Currently for Sudoku
// Doesn't work yet, can't completely randomize a sudoku problem as the computer will get stuck eventually
void GridManager::fillGrid()
{          
    srand(time(NULL));
    for (int y = 0; y < mSize; y++)
    {
        for (int x = 0; x < mSize; x++)
        {
            //printGrid();
            int temp = 1;
            std::vector<int> numAvailable = nums;
            // Check columns for repeating numbers
            while (x - temp >= 0)
            {
                // Remove the element using erase function and iterators 
                auto it = std::find(numAvailable.begin(), numAvailable.end(),
                    grid[x - temp][y]);

                // If element is found found, erase it 
                if (it != numAvailable.end()) {
                    numAvailable.erase(it);
                }
                temp++;
            }
           
            // Check rows for repeating numbers
            temp = 1;
            while (y - temp >= 0)
            {
                // Remove the element using erase function and iterators 
                auto it = std::find(numAvailable.begin(), numAvailable.end(),
                    grid[x][y - temp]);

                // If element is found found, erase it 
                if (it != numAvailable.end()) {
                    numAvailable.erase(it);
                }
                temp++;
            }
            // Checks subsquare for repeats
            for (int j = x % mSubSquareX; j >= 0; j--) // finds which column in subsquare x is at and goes left until it hits next subsquare/end for row y
            {
                // Remove the element using erase function and iterators 
                auto it = std::find(numAvailable.begin(), numAvailable.end(),
                    grid[x - j][y]);

                // If element is found found, erase it 
                if (it != numAvailable.end()) {
                    numAvailable.erase(it);
                }
            }
            for (int i = y % mSubSquareY; i > 0; i--) // starts at the beginning row of the current subsquare and goes up to the current row
            {
                for (int j = x - (x % mSubSquareX); j <= (x - (x % mSubSquareX)) + (mSubSquareX - 1); j++) // go through each column in the rows beneath the current row
                {
                    // Remove the element using erase function and iterators 
                    auto it = std::find(numAvailable.begin(), numAvailable.end(),
                        grid[j][y - i]);

                    // If element is found found, erase it 
                    if (it != numAvailable.end()) {
                        numAvailable.erase(it);
                    }
                }
            }
            if(numAvailable.size() != 0)
                grid[x][y] = numAvailable[rand() % numAvailable.size()];
        }
    }
}

void GridManager::printGrid()
{
    for (int i = 0; i < mSize; i++)
    {
        for (int j = 0; j < mSize; j++)
        {
            std::cout << grid[j][i] << " ";
            if (j % mSubSquareX == mSubSquareX - 1)
            {
                std::cout << "|";
            }
        }
        std::cout << std::endl;
        if (i % mSubSquareY == mSubSquareY - 1)
        {
            std::cout << "----------------------------------" << std::endl;
        }
    }

    std::cout << "\n\n";
}