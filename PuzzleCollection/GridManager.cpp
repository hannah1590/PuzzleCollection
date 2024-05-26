#include "GridManager.h";
GridManager::GridManager(int size, int boxX, int boxY)
{
    srand(time(NULL));

    mSize = size;
    mBoxSizeX = boxX;
    mBoxSizeY = boxY;

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
        nums.push_back(i + 1); // Fills the nums vector with all available number options
        vector<int> temp;
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


vectors fill from top to bottom then left to right
2D Vector = [y][x]
*/

// Currently for Sudoku
// Doesn't work yet, can't completely randomize a sudoku problem as the computer will get stuck eventually
void GridManager::fillGrid()
{
    vector<int> rowOptions;
    vector<int> colOptions;
    vector<int> boxOptions;
    vector<int> combinedOptions;

    for (int y = 0; y < mSize; y++)
    {
        for (int x = 0; x < mSize; x++)
        {
            rowOptions = checkRow(x, y);
            colOptions = checkColumn(x, y);
            boxOptions = checkBox(x, y);

            combinedOptions = combineVectors(rowOptions, colOptions);
            combinedOptions = combineVectors(combinedOptions, boxOptions);

            // Checks if there are no options
            if (combinedOptions.size() == 0)
            {
                if (rowOptions.size() == 1)
                {
                    bool loop = true;
                    int i = x - 1;
                    do {
                        vector<int> temp = checkColumn(i, y); // Gets all available options for column (i,y)
                        // Checks if the rowOption can be easily swapped to a different column
                        auto it = find(temp.begin(), temp.end(), rowOptions[0]);
                        if (it != temp.end())
                        {
                            grid[y][x] = grid[y][i];
                            grid[y][i] = rowOptions[0];
                            loop = false;
                        }
                        i--;
                    } while (loop && i >= 0);
                }
                else if (colOptions.size() == 1)
                {
                    bool loop = true;
                    int i = y - 1;
                    do {
                        vector<int> temp = checkRow(x, i); // Gets all available options for row (x,i)
                        // Checks if the colOption can be easily swapped to a different row
                        auto it = find(temp.begin(), temp.end(), colOptions[0]);
                        if (it != temp.end())
                        {
                            grid[y][x] = grid[i][x];
                            grid[i][x] = colOptions[0];
                            loop = false;
                        }
                        i--;
                    } while (loop && i >= 0);
                }
                else if (boxOptions.size() == 1)
                {
                    bool loop = true;
                    int i = x - 1;
                    do {
                        vector<int> temp = checkColumn(i, y); // Gets all available options for column (i,y)
                        // Checks if the rowOption can be easily swapped to a different column
                        auto it = find(temp.begin(), temp.end(), boxOptions[0]);
                        if (it != temp.end())
                        {
                            grid[y][x] = grid[y][i];
                            grid[y][i] = boxOptions[0];
                            loop = false;
                        }
                        i--;
                    } while (loop && i >= 0);
                    i = y - 1;
                    while (loop && i >= 0) {
                        vector<int> temp = checkRow(x, i); // Gets all available options for row (x,i)
                        // Checks if the colOption can be easily swapped to a different row
                        auto it = find(temp.begin(), temp.end(), boxOptions[0]);
                        if (it != temp.end())
                        {
                            grid[y][x] = grid[i][x];
                            grid[i][x] = boxOptions[0];
                            loop = false;
                        }
                        i--;
                    } 
                }
            }
            else
            {
                grid[y][x] = combinedOptions[rand() % combinedOptions.size()];
            }
        }
    }
}

vector<int> GridManager::combineVectors(vector<int> a, vector<int> b)
{
    vector<int> sameNums;
    for (auto& itA : a)
    {
        // Finds location of the number
        auto itB = find(b.begin(), b.end(), itA);

        // Remove number from available
        if (itB != b.end()) {
            sameNums.push_back(itA);
        }
    }
    return sameNums;
}

vector<int> GridManager::checkColumn(int x, int y)
{
    vector<int> numAvailable = nums;

    // Check columns for repeating numbers
    int temp = 1;
    while (y - temp >= 0)
    {
        // Finds location of the number
        auto it = find(numAvailable.begin(), numAvailable.end(),
            grid[y - temp][x]);

        // Remove number from available
        if (it != numAvailable.end()) {
            numAvailable.erase(it);
        }
        temp++;
    }
    return numAvailable;
}

vector<int> GridManager::checkRow(int x, int y)
{
    vector<int> numAvailable = nums;

    // Check rows for repeating numbers
    int temp = 1;
    while (x - temp >= 0)
    {
        // Finds location of the number
        auto it = find(numAvailable.begin(), numAvailable.end(),
            grid[y][x - temp]);

        // Remove number from available
        if (it != numAvailable.end()) {
            numAvailable.erase(it);
        }
        temp++;
    }
    return numAvailable;
}

vector<int> GridManager::checkBox(int x, int y)
{
    vector<int> numAvailable = nums;
    for (int i = y - (y % mBoxSizeY); i <= (y - (y % mBoxSizeY)) + (mBoxSizeY - 1); i++) // Starts at the beginning row of the current box and goes up to the current row
    {
        for (int j = x - (x % mBoxSizeX); j <= (x - (x % mBoxSizeX)) + (mBoxSizeX - 1); j++) // Go through each column in the rows beneath the current row
        {
            // Finds location of the number
            auto it = find(numAvailable.begin(), numAvailable.end(),
                grid[i][j]);

            // Remove number from available
            if (it != numAvailable.end()) {
                numAvailable.erase(it);
            }
        }
    }
    return numAvailable;
}

void GridManager::printGrid()
{
    for (int i = 0; i < mSize; i++)
    {
        for (int j = 0; j < mSize; j++)
        {
            cout << grid[i][j] << " ";
            if (j % mBoxSizeX == mBoxSizeX - 1)
            {
                cout << "|";
            }
        }
        cout << endl;
        if (i % mBoxSizeY == mBoxSizeY - 1)
        {
            cout << "----------------------------------" << endl;
        }
    }

    cout << "\n\n";
}


// Gets a randomly generated number that doesn't yet appear in the row/column/box; if one cannot be found returns 0
int GridManager::getAvailableNum(int x, int y)
{
    vector<int> numAvailable = nums;

    // Check rows for repeating numbers
    int temp = 1;
    while (y - temp >= 0)
    {
        // Finds location of the number
        auto it = find(numAvailable.begin(), numAvailable.end(),
            grid[x][y - temp]);

        // Remove number from available
        if (it != numAvailable.end()) {
            numAvailable.erase(it);
        }
        temp++;
    }


    // Check columns for repeating numbers
    temp = 1;
    while (x - temp >= 0)
    {
        // Finds location of the number
        auto it = find(numAvailable.begin(), numAvailable.end(),
            grid[x - temp][y]);

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
        auto it = find(numAvailable.begin(), numAvailable.end(),
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
            auto it = find(numAvailable.begin(), numAvailable.end(),
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