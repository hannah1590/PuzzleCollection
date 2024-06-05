#include "GridFiller.h";
GridFiller::GridFiller(int size, int boxX, int boxY)
{
    mSize = size;
    mBoxSizeX = boxX;
    mBoxSizeY = boxY;

    initGrid();
    //fillGrid();
    //printGrid();
}

// Clears grid
GridFiller::~GridFiller()
{
    for (auto& i : mGrid)
    {
        i.clear();
    }
    mGrid.clear();
    mNums.clear();
}

// Fills grid with 0s
void GridFiller::initGrid()
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
// Fills grid with randomly generated numbers that makes a full Sudoku board
// 2D Vector = [y][x]
void GridFiller::fillGrid()
{
    vector<int> rowOptions;
    vector<int> columnOptions;
    vector<int> boxOptions;
    vector<int> combinedOptions;
    stack<Vector2D> zeros;
    stack<Vector2D> toCheck;

    for (int y = 0; y < mSize; y++)
    {
        for (int x = 0; x < mSize; x++)
        {
           
            rowOptions = checkRow(x, y);
            columnOptions = checkColumn(x, y);
            boxOptions = checkBox(x, y);

            combinedOptions = combineVectors(rowOptions, columnOptions);
            combinedOptions = combineVectors(combinedOptions, boxOptions);

            // Checks if there are no options
            if (combinedOptions.size() == 0)
            {   
                zeros.push(Vector2D(x, y));
            }
            else
            {
                // If there are plenty of options, randomly choose from the ones available
                mGrid[y][x] = combinedOptions[rand() % combinedOptions.size()];
            }
        }
    }

    while (!toCheck.empty() || !zeros.empty())
    {
        Vector2D current;

        // Starts with the zeros
        if (zeros.empty())
        {
            current = toCheck.top();
            toCheck.pop();
        }
        else
        {
            current = zeros.top();
            zeros.pop();
        }
        int x = current.getX();
        int y = current.getY();

        // Checks if the number is a 0
        if (mGrid[y][x] == 0)
        {
            // Checks which number is missing in the box and puts it in the empty space
            boxOptions = checkBox(x, y);
            mGrid[y][x] = boxOptions[0];

            // Add it to the check stack to double check the rows and columns it is in
            toCheck.push(current); 
        }
        else
        {
            Vector2D newPos = findRepeatInColumn(x, y, mGrid[y][x]);

            // If the number repeats in the column
            if (newPos != Vector2D(-1, -1))
            {
                // Since it found where the number repeats in the same column it will have a different y but same x
                int nY = newPos.getY();

                // Go through the columns of the box
                for (int i = x - (x % mBoxSizeX); i <= (x - (x % mBoxSizeX)) + (mBoxSizeX - 1); i++)
                {
                    // Find the column that doesn't have the number
                    if (i != x && findRepeatInColumn(i, nY, mGrid[y][x]) == Vector2D(-1,-1))
                    {
                        // Swap the number into a new column of the same row that doesn't have that number
                        int holder = mGrid[nY][x];
                        mGrid[nY][x] = mGrid[nY][i];
                        mGrid[nY][i] = holder;

                        // Add the swapped positions to the stack to double check if they fit
                        toCheck.push(Vector2D(i, nY));
                        toCheck.push(Vector2D(x, nY));
                    }
                }
            }

            newPos = findRepeatInRow(x, y, mGrid[y][x]);

            // If the number repeats in the row
            if (newPos != Vector2D(-1, -1))
            {
                // Since it found where the number repeats in the same row it will have a different x but same y
                int nX = newPos.getX();

                // Go through the rows of the box
                for (int i = y - (y % mBoxSizeY); i <= (y - (y % mBoxSizeY)) + (mBoxSizeY - 1); i++)
                {
                    // Find the row that doesn't have the number
                    if (i != y && findRepeatInRow(nX, i, mGrid[y][x]) == Vector2D(-1, -1))
                    {
                        // Swap the number into a new row of the same column that doesn't have that number
                        int holder = mGrid[y][nX];
                        mGrid[y][nX] = mGrid[i][nX];
                        mGrid[i][nX] = holder;

                        // Add the swapped positions to the stack to double check if they fit
                        toCheck.push(Vector2D(nX, i));
                        toCheck.push(Vector2D(nX, y));
                    }
                }
            }
        }
    }
}

// Check which numbers are still available in a column
vector<int> GridFiller::checkColumn(int x, int y)
{
    vector<int> numAvailable = mNums;

    // Check from the current y to the beginning of the column
    int temp = 1;
    while (y - temp >= 0)
    {
        // Finds location of the number
        auto it = find(numAvailable.begin(), numAvailable.end(), mGrid[y - temp][x]);

        // Remove number from available
        if (it != numAvailable.end()) 
        {
            numAvailable.erase(it);
        }
        temp++;
    }
    return numAvailable;
}

// Check which numbers are still available for a row
vector<int> GridFiller::checkRow(int x, int y)
{
    vector<int> numAvailable = mNums;

    // Check from the current x to the beginning of the row
    int temp = 1;
    while (x - temp >= 0)
    {
        // Finds location of the number
        auto it = find(numAvailable.begin(), numAvailable.end(), mGrid[y][x - temp]);

        // Remove number from available
        if (it != numAvailable.end()) 
        {
            numAvailable.erase(it);
        }
        temp++;
    }
    return numAvailable;
}

// Check which numbers are still available for a box
vector<int> GridFiller::checkBox(int x, int y)
{
    vector<int> numAvailable = mNums;

    // Check through whole box
    for (int i = y - (y % mBoxSizeY); i <= (y - (y % mBoxSizeY)) + (mBoxSizeY - 1); i++)
    {
        for (int j = x - (x % mBoxSizeX); j <= (x - (x % mBoxSizeX)) + (mBoxSizeX - 1); j++) 
        {
            // Finds location of the number
            auto it = find(numAvailable.begin(), numAvailable.end(), mGrid[i][j]);

            // Remove number from available
            if (it != numAvailable.end()) 
            {
                numAvailable.erase(it);
            }
        }
    }
    return numAvailable;
}


// Combine two vector's like terms together into one vector
vector<int> GridFiller::combineVectors(vector<int> a, vector<int> b)
{
    vector<int> sameNums;
    for (auto& itA : a)
    {
        // Finds location of the number
        auto itB = find(b.begin(), b.end(), itA);

        // Remove number from available
        if (itB != b.end())
        {
            sameNums.push_back(itA);
        }
    }
    return sameNums;
}

// Go through the entire column to see if the number repeats
Vector2D GridFiller::findRepeatInColumn(int x, int y, int num)
{
    for (int i = 0; i < mSize; i++)
    {
        if (mGrid[i][x] == num && i != y)
        {
            return Vector2D(x, i);
        }
    }
    return Vector2D(-1, -1);
}

// Go through the entire row to see if the number repeats
Vector2D GridFiller::findRepeatInRow(int x, int y, int num)
{
    for (int i = 0; i < mSize; i++)
    {
        if (mGrid[y][i] == num && i != x)
        {
            return Vector2D(i, y);
        }
    }
    return Vector2D(-1, -1);
}

void GridFiller::printGrid()
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