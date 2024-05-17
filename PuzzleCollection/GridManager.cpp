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

// Currently for Sudoku
void GridManager::fillGrid()
{          
    
    srand(time(NULL));
    for (int y = 0; y < mSize; y++)
    {
        for (int x = 0; x < mSize; x++)
        {
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
            // Doesn't quite work yet, need to make sure to check the entire rows beneath the current coord
            std::cout << "pair: " << x << " " << y << std::endl;
            for (int i = y % mSubSquareY; i >= 0; i--) // finds which row in subsquare y is at and goes down until it hits next subsquare/end
            {
                for (int j = x % mSubSquareX; j >= 0; j--) // finds which column in subsquare x is at and goes left until it hits next subsquare/end
                {
                    std::cout << "(" << x - j << " " << y - i << ") ";

                    // Remove the element using erase function and iterators 
                    auto it = std::find(numAvailable.begin(), numAvailable.end(),
                        grid[x - j][y - i]);
                         
                    // If element is found found, erase it 
                    if (it != numAvailable.end()) {
                        numAvailable.erase(it);
                    }
                }
                std::cout << std::endl;
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
            std::cout << grid[i][j] << " ";
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
}