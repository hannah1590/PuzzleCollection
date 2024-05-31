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
        mNums.push_back(i + 1); // Fills the nums vector with all available number options
        vector<int> temp;
        for (int j = 0; j < mSize; j++)
        {
            temp.push_back(0);
        }
        mGrid.push_back(temp);
    }
}

// Currently for Sudoku
// 2D Vector = [y][x]
void GridManager::fillGrid()
{
    vector<int> rowOptions;
    vector<int> colOptions;
    vector<int> boxOptions;
    vector<int> combinedOptions;
    stack<Vector2D> toCheck;

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
            }
            else
            {
                // If there are plenty of options, randomly choose from the ones available
                mGrid[y][x] = combinedOptions[rand() % combinedOptions.size()];
            }
            
            // Makes a stack of positions that still end up zero as they can't be swapped while the grid is being made
            if (mGrid[y][x] == 0)
            {
                toCheck.push(Vector2D(x, y));
            }
        }
    }
    
    int count = 0;
    // Goes through every pos that needs to be adjusted
    while (!toCheck.empty() && count < 100)
    {
        Vector2D current = toCheck.top();
        toCheck.pop();

        // Checks if the number is a 0
        if (mGrid[current.getY()][current.getX()] == 0)
        {
            // Checks which number is missing in the box and puts it in the empty space
            boxOptions = checkBox(current.getX(), current.getY());
            mGrid[current.getY()][current.getX()] = boxOptions[0];
        }

        // Checks the entire row and column to find if it repeats
        rowOptions = checkRow(mSize, current.getY());
        colOptions = checkColumn(current.getX(), mSize);

        if (rowOptions.size() != 0)
        {
            for (int i = 0; i < mSize; i++)
            {
                // Finds the current pos of the repeated number
                if (mGrid[current.getY()][i] == mGrid[current.getY()][current.getX()] && i != current.getX())
                {
                    // Finds the position in the box of the number missing from the row 
                    Vector2D newPos = findNumInBox(i, current.getY(), rowOptions[0]);
                    mGrid[newPos.getY()][newPos.getX()] = mGrid[current.getY()][i];
                    mGrid[current.getY()][i] = rowOptions[0];

                    // Adds the positions of numbers just swapped to the stack
                    toCheck.push(Vector2D((float)i, current.getY()));
                    toCheck.push(newPos);
                    break;
                }
            }
        }
        if (colOptions.size() != 0)
        {
            for (int i = 0; i < mSize; i++)
            {
                // Finds the current pos of the repeated number
                if (mGrid[i][current.getX()] == mGrid[current.getY()][current.getX()] && i != current.getY())
                {
                    // Finds the position in the box of the number missing from the column
                    Vector2D newPos = findNumInBox(current.getX(), i, colOptions[0]);
                    mGrid[newPos.getY()][newPos.getX()] = mGrid[i][current.getX()];
                    mGrid[i][current.getX()] = colOptions[0];

                    // Adds the positions of numbers just swapped to the stack
                    toCheck.push(Vector2D(current.getX(), (float)i));
                    toCheck.push(newPos);
                    break;
                }
            }
        }
        count++;
    }
    
    // Need to fix it so I don't need to completely redo if it ends in an endless loop as this is very unefficient
    if (count == 100)
    {
        cout << "redo" << endl;
        for (int i = 0; i < mSize; i++)
        {
            for (int j = 0; j < mSize; j++)
            {
                mGrid[i][j] = 0;
            }
        }
        fillGrid();
    }
}

Vector2D GridManager::findNumInBox(int x, int y, int num)
{
    Vector2D zero;
    for (int i = y - (y % mBoxSizeY); i <= (y - (y % mBoxSizeY)) + (mBoxSizeY - 1); i++) // Starts at the beginning row of the current box and goes up to the current row
    {
        for (int j = x - (x % mBoxSizeX); j <= (x - (x % mBoxSizeX)) + (mBoxSizeX - 1); j++) // Go through each column in the rows beneath the current row
        {
            // Finds pos of number
            if (mGrid[i][j] == num && Vector2D(j, i) != Vector2D(x, y))
            {
                return Vector2D(j, i);
            }
            else if (mGrid[i][j] == 0)
            {
                zero = Vector2D(j, i);
            }
        }
    }
    // If the number isn't in the box but there is a zero, returns the pos of the zero
    return zero;
}

// Checks throughout the available options of a specific row/column/box and sees if it can be switched to an earlier position
bool GridManager::switchNumbers(vector<int> holder, vector<int> options, vector<int> ogAvail, int x, int y, int a, int b)
{
    for (auto& i : options)
    {
        auto it = find(holder.begin(), holder.end(), i);
        auto it2 = find(ogAvail.begin(), ogAvail.end(), mGrid[b][a]);
        if (it != holder.end() && it2 != ogAvail.end())
        {
            mGrid[y][x] = mGrid[b][a];
            mGrid[b][a] = i;
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
    vector<int> numAvailable = mNums;

    // Check columns for repeating numbers
    int temp = 1;
    while (y - temp >= 0)
    {
        // Finds location of the number
        auto it = find(numAvailable.begin(), numAvailable.end(), mGrid[y - temp][x]);

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
    vector<int> numAvailable = mNums;

    // Check rows for repeating numbers
    int temp = 1;
    while (x - temp >= 0)
    {
        // Finds location of the number
        auto it = find(numAvailable.begin(), numAvailable.end(), mGrid[y][x - temp]);

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
    vector<int> numAvailable = mNums;
    for (int i = y - (y % mBoxSizeY); i <= (y - (y % mBoxSizeY)) + (mBoxSizeY - 1); i++) // Starts at the beginning row of the current box and goes up to the current row
    {
        for (int j = x - (x % mBoxSizeX); j <= (x - (x % mBoxSizeX)) + (mBoxSizeX - 1); j++) // Go through each column in the rows beneath the current row
        {
            // Finds location of the number
            auto it = find(numAvailable.begin(), numAvailable.end(), mGrid[i][j]);

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
            cout << mGrid[i][j] << " ";
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