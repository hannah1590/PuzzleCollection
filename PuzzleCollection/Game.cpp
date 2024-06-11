#include "Game.h"

Game* Game::mspInstance = nullptr;

Game::Game()
{
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

	mHUD->loadColorData(mTextColor, mTileColor, mNoteUIColor);
	mHUD->loadFontData(ASSET_FILE_PATH, mFontName, mMenuFontSize, mNumberFontSize);

	mMenuManager->loadColorData(mTextColor);
	mMenuManager->loadFontData(ASSET_FILE_PATH, mFontName, mMenuFontSize, mSmallMenuFontSize);

	mGridManager->loadGridVariables(mGridSize, mBoxSizeX, mBoxSizeY, mPercentRemoved, mTilePadding, mNotePadding);
	mGridManager->loadColorData(mDefaultNumberColor, mPlayerInputNumberColor, mSameNumberColor, mWrongInputColor);
	mGridManager->loadFontData(ASSET_FILE_PATH, mFontName, mNumberFontSize, mNoteFontSize);
}

// End game
Game::~Game()
{
	save();
	cleanup();
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
		>> holder >> mNotePadding;
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

void Game::init()
{
	// Inits all game systems
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

	// Add game events
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

	// Loading Menu/HUD text
	mMenuManager->loadData(MAIN_FILE_PATH + MENU_TEXT_FILE);
	mHUD->loadData(MAIN_FILE_PATH + MENU_TEXT_FILE);

	mIsInitted = true;

	// Adding to graphics buffer
	GraphicsBuffer* pBlackBuffer = new GraphicsBuffer(DISP_WIDTH, DISP_HEIGHT);
	GraphicsBuffer* pTileBuffer = new GraphicsBuffer(mTileColor, mTileSize, mTileSize);

	// Buffers for the lines separating each box
	GraphicsBuffer* pXSeparatorBuffer = new GraphicsBuffer(mBorderColor, mGridSize * (mTileSize + mTilePadding) + mTilePadding, mTilePadding);
	GraphicsBuffer* pYSeparatorBuffer = new GraphicsBuffer(mBorderColor, mTilePadding, mGridSize * (mTileSize + mTilePadding) + mTilePadding);

	// Buffers for a box highlighting the current tile clicked
	GraphicsBuffer* pXHighlightBuffer = new GraphicsBuffer(mHighlightColor, mTileSize + (mTilePadding * 2), mTilePadding);
	GraphicsBuffer* pYHighlightBuffer = new GraphicsBuffer(mHighlightColor, mTilePadding, mTileSize + (mTilePadding * 2));

	assert(pBlackBuffer && pTileBuffer && pXSeparatorBuffer && pYSeparatorBuffer && pXHighlightBuffer && pYHighlightBuffer);

	mGraphicsSystem->setBitmapToColor(*pBlackBuffer, mBackgroundColor);

	int bufferIndex = 0;

	// Add each graphics buffer to the manager
	mGraphicsBufferManager->addBuffer(bufferIndex, *pBlackBuffer);
	mBackgroundIndex = bufferIndex; bufferIndex++;
	mGraphicsBufferManager->addBuffer(bufferIndex, *pTileBuffer);
	mTileIndex = bufferIndex; bufferIndex++;
	mGraphicsBufferManager->addBuffer(bufferIndex, *pXSeparatorBuffer);
	mXSeparatorIndex = bufferIndex; bufferIndex++;
	mGraphicsBufferManager->addBuffer(bufferIndex, *pYSeparatorBuffer);
	mYSeparatorIndex = bufferIndex; bufferIndex++;
	mGraphicsBufferManager->addBuffer(bufferIndex, *pXHighlightBuffer);
	mXHighlightIndex = bufferIndex; bufferIndex++;
	mGraphicsBufferManager->addBuffer(bufferIndex, *pYHighlightBuffer);
	mYHighlightIndex = bufferIndex; bufferIndex++;

	// BGM
	mSoundManager->playSample(true, mMusicIndex);

	// Init grid
	mGridFiller->initGrid(mGridSize, mBoxSizeX, mBoxSizeY);
	mGridManager->init(*mGraphicsBufferManager, mTileIndex);
	// Init HUD
	mHUD->init(*mGraphicsBufferManager, mTileIndex, mGridSize, mTilePadding);
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

// Main game loop
void Game::doLoop()
{
	PerformanceTracker* pPerformanceTracker = new PerformanceTracker;
	const double frameRate = 16.7;

	// For grid
	srand((unsigned int)time(NULL));
	
	// Load up grid
	mGridManager->loadGrid(SUDOKU, DISP_WIDTH, DISP_HEIGHT);

	while (mIsLooping)
	{
		// Start loop timer
		pPerformanceTracker->clearTracker("loop");
		pPerformanceTracker->startTracking("loop");
		Timer timer;
		timer.start();
		
		if (!mMenuManager->getIsMenuOpen())
		{
			// If a menu is not open draw what is needed for the game
			mInputSystem->updateEvents();
			mGraphicsSystem->drawBackbuffer(Vector2D(0, 0), *mGraphicsBufferManager->getBuffer(mBackgroundIndex), 1.0);
			mGridManager->draw(mXSeparatorIndex, mYSeparatorIndex, mTileIndex, mXHighlightIndex, mYHighlightIndex);

			mHUD->update(mSavedTime, mNotesOn); // adjust for when the notes are open or not
			mGraphicsSystem->flip();
		}

		if (mMenuManager->getIsMenuOpen())
		{
			// If a menu is open do not draw game but draw what is needed for the menu
			mHUD->pauseTimer();
			mInputSystem->updateEvents();
			mGraphicsSystem->drawBackbuffer(Vector2D(0, 0), *mGraphicsBufferManager->getBuffer(mBackgroundIndex), 1.0);
			mMenuManager->draw();

			mGraphicsSystem->flip();
		}

		timer.sleepUntilElapsed(frameRate);
		pPerformanceTracker->stopTracking("loop");
	}
	delete pPerformanceTracker;
}

// Recieves and applies events
void Game::handleEvent(const Event& theEvent)
{
	const GameEvent& gameEvent = static_cast<const GameEvent&>(theEvent);

	if (gameEvent.getType() == START_SOUND_EVENT) { mSoundManager->soundOn(); }
	if (gameEvent.getType() == STOP_SOUND_EVENT) { mSoundManager->soundOff(); }

	if (gameEvent.getType() == START_STOP_EVENT)
	{
		mMenuManager->toggleMenu();
	}
	// TO DO: reset current game
	if (gameEvent.getType() == NEW_GAME_EVENT)
	{
		mHUD->reset();
	}
	if (gameEvent.getType() == CHANGE_DIFFICULTY_EVENT)
	{
		//mPlayerManager->setDifficulty(mpMenuManager->getCurrentDifficulty());
		int currentDiff = mMenuManager->getCurrentDifficulty();
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
	if (gameEvent.getType() == CHANGE_GRID_SIZE_EVENT)
	{
		int currentGrid = mMenuManager->getCurrentGridSize();
		if (currentGrid == 0)
		{
			mGridSize = 9;
			mBoxSizeX = 3;
			mBoxSizeY = 3;
		}
		else if (currentGrid == 1)
		{
			mGridSize = 6;
			mBoxSizeX = 3;
			mBoxSizeY = 2;
		}
	}
	if (gameEvent.getType() == SCREEN_CLICK_EVENT)
	{
		if (mMenuManager->getIsMenuOpen() == true)
		{
			mMenuManager->checkInput(gameEvent.getMouseLocation());
		}
		else
		{
			int num;
			num = mHUD->checkInput(gameEvent.getMouseLocation());
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
	if (gameEvent.getType() == OPEN_CLOSE_NOTES_EVENT)
	{
		mNotesOn = !mNotesOn;
	}
}

Game* Game::getInstance() 
{
	assert(mspInstance);
	return mspInstance;
}

void Game::initInstance() 
{
	assert(!mspInstance);
	mspInstance = new Game();
}

void Game::cleanupInstance()
{
	delete mspInstance;
	mspInstance = nullptr;
}
