#include "Game.h"

// Single game instace
Game* Game::mspInstance = nullptr;

// Sets up all game variables and data
Game::Game()
{
	// Loads specific data from files
	getVariables();
	getColors();
	getFontData();

	// Initialize all game systems
	mInputSystem = new InputSystem;
	mGraphicsSystem = new GraphicsSystem;
	mGraphicsBufferManager = new GraphicsBufferManager;
	mUnitManager = new UnitManager;
	mInputTranslator = new InputTranslator;
	mHUD = new HUD(*mGraphicsSystem, DISP_WIDTH, DISP_HEIGHT);
	mSoundManager = new SoundManager();
	mMenuManager = new MenuManager(*mGraphicsSystem);
	mGridFiller = new GridFiller(); 
	mGridManager = new GridManager(*mGraphicsSystem, *mGridFiller);

	// Provides specific data to certain classes that need them
	mHUD->loadColorData(mTextColor, mTileColor, mNoteUIColor);
	mHUD->loadFontData(ASSET_FILE_PATH, mFontName, mMenuFontSize, mNumberFontSize);

	mMenuManager->loadColorData(mTextColor);
	mMenuManager->loadFontData(ASSET_FILE_PATH, mFontName, mMenuFontSize, mSmallMenuFontSize);

	mGridManager->loadColorData(mDefaultNumberColor, mPlayerInputNumberColor, mSameNumberColor, mWrongInputColor);
	mGridManager->loadFontData(ASSET_FILE_PATH, mFontName, mNumberFontSize, mNoteFontSize);
}

// End game
Game::~Game()
{
	save();
	cleanup();
}

// Inits all game systems
void Game::init()
{
	if (!mInputSystem->init())
	{
		cout << "ERROR - Input system installation failed\n";
		system("pause");
	}

	if (!mGraphicsSystem->initGraphics())
	{
		cout << "ERROR - Init failed\n";
		system("pause");
	}

	// No need to pause since sound isn't game breaking
	if (!mSoundManager->init(MAIN_FILE_PATH + SOUND_LIST_FILE, ASSET_FILE_PATH + ""))
	{
		cout << "ERROR - Sound init failed\n";
	}

	mGraphicsSystem->createDisplay(DISP_WIDTH, DISP_HEIGHT);

	// Make sure only one instance of game exists
	if (mIsInitted)
	{
		cleanup();
	}

	mIsInitted = true;

	// Add game events so they fire to this class
	EventSystem* pEventSystem = EventSystem::getInstance();
	pEventSystem->addListener((EventType)START_STOP_EVENT, this);
	pEventSystem->addListener((EventType)NEW_GAME_EVENT, this);
	pEventSystem->addListener((EventType)CHANGE_DIFFICULTY_EVENT, this);
	pEventSystem->addListener((EventType)QUITTING_EVENT, this);
	pEventSystem->addListener((EventType)SCREEN_CLICK_EVENT, this);
	pEventSystem->addListener((EventType)STOP_SOUND_EVENT, this);
	pEventSystem->addListener((EventType)START_SOUND_EVENT, this);
	pEventSystem->addListener((EventType)OPEN_CLOSE_NOTES_EVENT, this);
	pEventSystem->addListener((EventType)CHANGE_GRID_SIZE_EVENT, this);
	pEventSystem->addListener((EventType)ADD_SCORE_EVENT, this);
	pEventSystem->addListener((EventType)MINUS_SCORE_EVENT, this);
	pEventSystem->addListener((EventType)WIN_GAME_EVENT, this);

	// Loading Menu/HUD text
	mMenuManager->loadData(MAIN_FILE_PATH + MENU_TEXT_FILE);
	mHUD->loadData(MAIN_FILE_PATH + MENU_TEXT_FILE);

	// Adding to graphics buffer
	GraphicsBuffer* pBlackBuffer = new GraphicsBuffer(DISP_WIDTH, DISP_HEIGHT);
	GraphicsBuffer* pTileBuffer = new GraphicsBuffer(mTileColor, mTileSize, mTileSize);

	assert(pBlackBuffer && pTileBuffer);

	// Sets up black background
	mGraphicsSystem->setBitmapToColor(*pBlackBuffer, mBackgroundColor);

	// Add each graphics buffer to the manager
	int bufferIndex = 0;
	mGraphicsBufferManager->addBuffer(bufferIndex, *pBlackBuffer);
	mBackgroundIndex = bufferIndex; bufferIndex++;
	mGraphicsBufferManager->addBuffer(bufferIndex, *pTileBuffer);
	mTileIndex = bufferIndex; bufferIndex++;

	// BGM
	//mSoundManager->playSample(true, mMusicIndex);

	// Init grid manager
	mGridManager->init(*mGraphicsBufferManager, mTileIndex);
}

// Delete game systems in opposite order of creation
void Game::cleanup()
{
	delete mGridManager;
	delete mGridFiller;
	delete mMenuManager;
	delete mSoundManager;
	delete mHUD;
	delete mInputTranslator;
	delete mUnitManager;
	delete mGraphicsBufferManager;
	delete mGraphicsSystem;
	delete mInputSystem;
	mIsInitted = false;
}

// Create a save file and load important variables
void Game::save()
{

}

// Load saved variables
void Game::loadSave()
{

}

// Initialize game variables from file
void Game::getVariables()
{
	ifstream gameVariables(MAIN_FILE_PATH + GAME_VARIABLES_FILE);
	if (!gameVariables.is_open())
	{
		cout << "Error getting GameVariables file" << endl;
	}
	string holder;

	// Ignores variable label and just gets number
	gameVariables >> holder >> mGridSize
		>> holder >> mBoxSizeX
		>> holder >> mBoxSizeY
		>> holder >> mTileSize
		>> holder >> mPercentRemoved
		>> holder >> mTilePadding
		>> holder >> mNotePadding
		>> holder >> mScoreAdd
		>> holder >> mScoreMinus;
	gameVariables.close();
}

// Get color variables from file
void Game::getColors()
{
	ifstream colorFile(MAIN_FILE_PATH + COLOR_DATA_FILE);
	if (!colorFile.is_open())
	{
		cout << "Error getting Color file" << endl;
	}
	string holder;
	int r, g, b, a;

	getline(colorFile, holder);

	// Get the values for each color and assign it to the correct color variable
	colorFile >> holder >> r >> g >> b >> a;
	mBackgroundColor.setValues(r, g, b, a);

	colorFile >> holder >> r >> g >> b >> a;
	mTileColor.setValues(r, g, b, a);

	colorFile >> holder >> r >> g >> b >> a;
	mTextColor.setValues(r, g, b, a);

	colorFile >> holder >> r >> g >> b >> a;
	mBorderColor.setValues(r, g, b, a);

	colorFile >> holder >> r >> g >> b >> a;
	mHighlightColor.setValues(r, g, b, a);

	colorFile >> holder >> r >> g >> b >> a;
	mDefaultNumberColor.setValues(r, g, b, a);

	colorFile >> holder >> r >> g >> b >> a;
	mPlayerInputNumberColor.setValues(r, g, b, a);

	colorFile >> holder >> r >> g >> b >> a;
	mSameNumberColor.setValues(r, g, b, a);

	colorFile >> holder >> r >> g >> b >> a;
	mWrongInputColor.setValues(r, g, b, a);

	colorFile >> holder >> r >> g >> b >> a;
	mNoteUIColor.setValues(r, g, b, a);

	colorFile.close();
}

// Get font data from file
void Game::getFontData()
{
	ifstream fontFile(MAIN_FILE_PATH + FONT_DATA_FILE);
	if (!fontFile.is_open())
	{
		cout << "Error getting Color file" << endl;
	}
	string holder;

	fontFile >> holder >> mFontName
		>> holder >> mMenuFontSize
		>> holder >> mSmallMenuFontSize
		>> holder >> mNumberFontSize
		>> holder >> mNoteFontSize;

	fontFile.close();
}

// Main game loop
void Game::doLoop()
{
	// Tracks game time
	PerformanceTracker* pPerformanceTracker = new PerformanceTracker;
	const double frameRate = 16.7;

	// For grid
	srand((unsigned int)time(NULL));
	
	// Goes through game loop until told to stop
	while (mIsLooping)
	{
		// Start loop timer
		pPerformanceTracker->clearTracker("loop");
		pPerformanceTracker->startTracking("loop");
		Timer timer;
		timer.start();
		
		// If a menu is not open draw what is needed for the game
		if (!mMenuManager->getIsMenuOpen())
		{
			mInputSystem->updateEvents();
			mGraphicsSystem->drawBackbuffer(Vector2D(0, 0), *mGraphicsBufferManager->getBuffer(mBackgroundIndex), 1.0);
			mGridManager->draw(mXSeparatorIndex, mYSeparatorIndex, mTileIndex, mXHighlightIndex, mYHighlightIndex);

			mHUD->update(mSavedTime, mNotesOn);
			mGraphicsSystem->flip();
		}

		// If a menu is open do not draw game but draw what is needed for the menu
		if (mMenuManager->getIsMenuOpen())
		{
			mHUD->pauseTimer();
			mInputSystem->updateEvents();
			mGraphicsSystem->drawBackbuffer(Vector2D(0, 0), *mGraphicsBufferManager->getBuffer(mBackgroundIndex), 1.0);
			mMenuManager->draw();

			mGraphicsSystem->flip();
		}

		// Stops tracking loop until next one starts
		timer.sleepUntilElapsed(frameRate);
		pPerformanceTracker->stopTracking("loop");
	}
	delete pPerformanceTracker;
}

// Recieves and applies game events
void Game::handleEvent(const Event& theEvent)
{
	const GameEvent& gameEvent = static_cast<const GameEvent&>(theEvent);

	// Turns sound on/off if button was pushed
	if (gameEvent.getType() == START_SOUND_EVENT) { mSoundManager->soundOn(); }
	if (gameEvent.getType() == STOP_SOUND_EVENT) { mSoundManager->soundOff(); }

	// Starts or stops the game
	if (gameEvent.getType() == START_STOP_EVENT)
	{
		mMenuManager->toggleMenu();
	}

	// Starts a new game
	if (gameEvent.getType() == NEW_GAME_EVENT)
	{
		// Clears grid
		mGridManager->clearGrid();
		mGridFiller->clearGrid();

		// Deletes buffers if they already exist
		if (mXSeparatorIndex != 0)
		{
			mGraphicsBufferManager->deleteBuffer(mXSeparatorIndex);
			mGraphicsBufferManager->deleteBuffer(mYSeparatorIndex);
			mGraphicsBufferManager->deleteBuffer(mXHighlightIndex);
			mGraphicsBufferManager->deleteBuffer(mYHighlightIndex);
		}

		// Makes grid-based buffers
		// Buffers for the lines separating each box
		GraphicsBuffer* pXSeparatorBuffer = new GraphicsBuffer(mBorderColor, mGridSize * (mTileSize + mTilePadding) + mTilePadding, mTilePadding);
		GraphicsBuffer* pYSeparatorBuffer = new GraphicsBuffer(mBorderColor, mTilePadding, mGridSize * (mTileSize + mTilePadding) + mTilePadding);

		// Buffers for a box highlighting the current tile clicked
		GraphicsBuffer* pXHighlightBuffer = new GraphicsBuffer(mHighlightColor, mTileSize + (mTilePadding * 2), mTilePadding);
		GraphicsBuffer* pYHighlightBuffer = new GraphicsBuffer(mHighlightColor, mTilePadding, mTileSize + (mTilePadding * 2));

		assert(pXSeparatorBuffer && pYSeparatorBuffer && pXHighlightBuffer && pYHighlightBuffer);

		// Add buffers to buffer manager
		int bufferIndex = mTileIndex + 1;
		mGraphicsBufferManager->addBuffer(bufferIndex, *pXSeparatorBuffer);
		mXSeparatorIndex = bufferIndex; bufferIndex++;
		mGraphicsBufferManager->addBuffer(bufferIndex, *pYSeparatorBuffer);
		mYSeparatorIndex = bufferIndex; bufferIndex++;
		mGraphicsBufferManager->addBuffer(bufferIndex, *pXHighlightBuffer);
		mXHighlightIndex = bufferIndex; bufferIndex++;
		mGraphicsBufferManager->addBuffer(bufferIndex, *pYHighlightBuffer);
		mYHighlightIndex = bufferIndex; bufferIndex++;
		
		// Init grid
		mGridFiller->initGrid(mGridSize, mBoxSizeX, mBoxSizeY);
		mGridManager->loadGridVariables(mGridSize, mBoxSizeX, mBoxSizeY, mPercentRemoved, mTilePadding, mNotePadding);
		mGridManager->loadGrid(SUDOKU, DISP_WIDTH, DISP_HEIGHT);

		// Init HUD
		mHUD->reset();
		mHUD->init(*mGraphicsBufferManager, mTileIndex, mGridSize, mTilePadding);
	}

	// Change the difficulty of the game
	if (gameEvent.getType() == CHANGE_DIFFICULTY_EVENT)
	{
		int currentDiff = mMenuManager->getCurrentDifficulty();
		int gridSize = mMenuManager->getCurrentGridSize();

		// Percent removed per difficulty changes between 9x9 size and 6x6 size
		if (gridSize == 0)
		{
			if (currentDiff == 0)
			{
				mPercentRemoved = 40;
			}
			else if (currentDiff == 1)
			{
				mPercentRemoved = 60;
			}
			else if (currentDiff == 2)
			{
				mPercentRemoved = 80;
			}
		}
		else
		{
			if (currentDiff == 0)
			{
				mPercentRemoved = 20;
			}
			else if (currentDiff == 1)
			{
				mPercentRemoved = 40;
			}
			else if (currentDiff == 2)
			{
				mPercentRemoved = 60;
			}
		}
	}

	// Changes whether grid is 9x9 or 6x6
	if (gameEvent.getType() == CHANGE_GRID_SIZE_EVENT)
	{
		int currentGrid = mMenuManager->getCurrentGridSize();

		if (currentGrid == 0)
		{
			// If player is actually changing from 6x6 to 9x9, add 20 to percent removed as 9x9 difficulty has 20 percent more than 6x6
			if (mGridSize != 9)
				mPercentRemoved += 20;

			mGridSize = 9;
			mBoxSizeX = 3;
			mBoxSizeY = 3;
		}
		else if (currentGrid == 1)
		{
			// If player is actually changing from 9x9 to 6x6, remove 20 from percent removed as 6x6 difficulty has 20 percent less than 9x9
			if (mGridSize != 6)
				mPercentRemoved -= 20;

			mGridSize = 6;
			mBoxSizeX = 3;
			mBoxSizeY = 2;
		}
	}

	// If user clicked on the screen
	if (gameEvent.getType() == SCREEN_CLICK_EVENT)
	{
		// If menu is open, check if a menu button was clicked
		if (mMenuManager->getIsMenuOpen() == true)
		{
			mMenuManager->checkInput(gameEvent.getMouseLocation());
		}
		else
		{
			// First check if the number UI at the bottom of the screen was clicked
			int num;
			num = mHUD->checkInput(gameEvent.getMouseLocation());

			// If number UI was clicked, adjust the tile depending on if notes is active; otherwise check if a grid tile was clicked
			if (num != 0)
			{
				if (!mHUD->getNotesOn())
					mGridManager->changeValue(num);
				else
					mGridManager->addNote(num);
			}
			else
				mGridManager->checkInput(gameEvent.getMouseLocation());
		}
	}

	// Player closed the game
	if (gameEvent.getType() == QUITTING_EVENT)
	{
		//hitting escape while the game is playing doesn't do anything
		if (mMenuManager->getIsMenuOpen() == true)
		{
			if (mMenuManager->getCurrentMenu() != 0)
			{
				mMenuManager->setToMain();
			}
			else
			{
				if (mMenuManager->shouldQuit())
				{
					mIsLooping = false;
				}
			}
		}
	}

	// Turns the note function on/off
	if (gameEvent.getType() == OPEN_CLOSE_NOTES_EVENT)
	{
		mNotesOn = !mNotesOn;
	}

	// Adds score when player does a correct input
	if (gameEvent.getType() == ADD_SCORE_EVENT)
	{
		Vector2D pos = mGridManager->getHighlightGridPos();
		int x = pos.getX();
		int y = pos.getY();

		// Add extra score if it is the completion of a part of the puzzle
		int scoreAdd = mScoreAdd;
		if (mGridManager->isRowCompleted(y))
			scoreAdd += mScoreAdd;
		if (mGridManager->isColumnCompleted(x))
			scoreAdd += mScoreAdd;
		if (mGridManager->isBoxCompleted(x,y))
			scoreAdd += mScoreAdd;

		// Add extra score if it completes all 3 parts
		if (scoreAdd / 4 == mScoreAdd)
			scoreAdd += mScoreAdd;

		mHUD->addScore(scoreAdd);
	}

	// Subtracts score if player has an incorrect input
	if (gameEvent.getType() == MINUS_SCORE_EVENT)
	{
		mHUD->minusScore(mScoreMinus);
	}

	// When entire grid is filled out, win game
	if (gameEvent.getType() == WIN_GAME_EVENT)
	{
		mMenuManager->setToWin(mHUD->getScore());
	}
}

// Gets single instance of Game
Game* Game::getInstance() 
{
	assert(mspInstance);
	return mspInstance;
}

// Initializes single Game instance
void Game::initInstance() 
{
	assert(!mspInstance);
	mspInstance = new Game();
}

// Deletes Game instance
void Game::cleanupInstance()
{
	delete mspInstance;
	mspInstance = nullptr;
}
