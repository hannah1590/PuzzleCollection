#include "GridManager.h";
void GridManager::checkGrid()
{
    for (int y = 0; y < mSize; y++)
    {
        int sum = 0;
        for (int x = 0; x < mSize; x++)
        {
            sum += grid[y][x];
        }
        cout << "Row sum: " << sum << endl;
    }

    for (int x = 0; x < mSize; x++)
    {
        int sum = 0;
        for (int y = 0; y < mSize; y++)
        {
            sum += grid[y][x];
        }
        cout << "Col sum: " << sum << endl;
    }
}

GridManager::GridManager(int size, int boxX, int boxY)
{
    srand(time(NULL));

    mSize = size;
    mBoxSizeX = boxX;
    mBoxSizeY = boxY;

    initGrid();
    fillGrid();
    checkGrid();
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
                        vector<int> temp1 = checkBox(i, y); // Gets all available options for box (i,y)
                        temp = combineVectors(temp, temp1);
                        loop = switchNumbers(temp, rowOptions, combineVectors(colOptions, boxOptions), x, y, i, y);
                        i--;
                    } while (loop && i >= 0);
                }
                else if (colOptions.size() == 1) // might be useless since the rows are completely filled so won't find available num
                {
                    bool loop = true;
                    int i = y - 1;
                    do {
                        vector<int> temp = checkRow(x, i); // Gets all available options for row (x,i)
                        vector<int> temp1 = checkBox(x, i); // Gets all available options for box (x,i)
                        temp = combineVectors(temp, temp1);
                        loop = switchNumbers(temp, colOptions, combineVectors(rowOptions, boxOptions), x, y, x, i);
                        i--;
                    } while (loop && i >= 0);
                }
                else
                { 
                    //if(boxOptions.size() != 0)
                    grid[y][x] = boxOptions[rand() % boxOptions.size()];
                    for (int i = y - 1; i >= 0; i--)
                    {
                        if (grid[i][x] == grid[y][x])
                        {
                            reshuffleBox(x, i);
                        }
                    }
                    
                   /*
                    // Checks if last option can be swapped with another number in the column
                    bool loop = true;
                    int i = x - 1;
                    do {
                        vector<int> temp = checkColumn(i, y); // Gets all available options for column (i,y)
                        vector<int> temp1 = checkRow(i, y); // Gets all available options for box (x,i)
                        temp = combineVectors(temp, temp1);
                        loop = switchNumbers(temp, boxOptions, combineVectors(colOptions, rowOptions), x, y, i, y);
                        i--;
                    } while (loop && i >= 0);

                    // If can't be swapped in the column, check if it can be swapped in the row
                    i = y - 1;
                    while (loop && i >= 0) {
                        vector<int> temp = checkRow(x, i); // Gets all available options for row (x,i)
                        vector<int> temp1 = checkColumn(x, i); // Gets all available options for box (x,i)
                        temp = combineVectors(temp, temp1);
                        loop = switchNumbers(temp, boxOptions, combineVectors(colOptions, rowOptions), x, y, x, i);
                        i--;
                    } 
                    */
                }
            }
            else
            {
                // If there are plenty of options, randomly choose from the ones available
                grid[y][x] = combinedOptions[rand() % combinedOptions.size()];
            }
        }
    }
}

void GridManager::reshuffleBox(int x, int y)
{
    vector<int> rowOptions;
    vector<int> colOptions;
    vector<int> boxOptions;
    vector<int> combinedOptions;

    for (int i = y - (y % mBoxSizeY); i <= (y - (y % mBoxSizeY)) + (mBoxSizeY - 1); i++)
    {
        for (int j = x - (x % mBoxSizeX); j <= (x - (x % mBoxSizeX)) + (mBoxSizeX - 1); j++)
        {
            rowOptions = checkRow(mSize, i);
            colOptions = checkColumn(j, mSize);
            boxOptions = checkBox(j, i);

            combinedOptions = combineVectors(rowOptions, colOptions);
            combinedOptions = combineVectors(combinedOptions, boxOptions);

            if (combinedOptions.size() != 0)
                grid[i][j] = combinedOptions[rand() % combinedOptions.size()];
        }
    }
}

// Checks throughout the available options of a specific row/column/box and sees if it can be switched to an earlier position
bool GridManager::switchNumbers(vector<int> holder, vector<int> options, vector<int> ogAvail, int x, int y, int a, int b)
{
    for (auto& i : options)
    {
        auto it = find(holder.begin(), holder.end(), i);
        auto it2 = find(ogAvail.begin(), ogAvail.end(), grid[b][a]);
        if (it != holder.end() && it2 != ogAvail.end())
        {
            grid[y][x] = grid[b][a];
            grid[b][a] = i;
            return false;
        }
    }
    return true;
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