#include "Game.h"

Game* Game::mspInstance = nullptr;

Game::Game()
{
	getVariables();
	getSounds();

	// Initialize all game systems
	mInputSystem = new InputSystem;
	mGraphicsSystem = new GraphicsSystem;
	mGraphicsBufferManager = new GraphicsBufferManager;
	mUnitManager = new UnitManager;
	mInputTranslator = new InputTranslator;
	mHUD = new HUD(*mGraphicsSystem, DISP_WIDTH, DISP_HEIGHT);
	mSoundManager = new SoundManager(mMaxSamples);
	mMenuManager = new MenuManager(*mGraphicsSystem);
	mGridFiller = new GridFiller(mGridSize, mBoxSizeX, mBoxSizeY); 
	mGridManager = new GridManager(*mGraphicsSystem, *mGridFiller);
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
	// None of these variables are currently relevent
	/*
	ifstream gameVariables(FILE_PATH + GAME_VARIABLES_FILENAME);
	if (!gameVariables.is_open())
	{
		cout << "Error getting GameVariables file" << endl;
	}
	gameVariables >> mMinVelocity >> mMaxVelocity >>
		mMinSpeed >> mMaxSpeed >>
		mMaxSpeedCap >> mMinSpeedCap >>
		mSpawnRadius >> mSpawnRadiusCap >>
		mRandomChance >> mRandomChanceCap >>
		mSecondsUntilIncrease >> mSpeedIncreaseRate >>
		mSpawnIncreaseRate >> mRandomChanceIncreaseRate >> 
		mPoints >> mPointIncrease >> mPointDecrease;
	gameVariables.close();*/
}

// Initialize sound wavs from file
void Game::getSounds()
{
	ifstream soundFiles(FILE_PATH + SOUND_FILENAMES_LOCATION);
	if (!soundFiles.is_open())
	{
		cout << "Error getting SoundEffects file" << endl;
	}
	getline(soundFiles, mDeathSound);
	getline(soundFiles, mGameOverSound);
	getline(soundFiles, mChangeSpriteSound);
	getline(soundFiles, mSpawnSound);
	soundFiles >> mMaxSamples;
	soundFiles.close();
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
	if (!mSoundManager->init())
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

	//loading language
	mMenuManager->loadData(FILE_PATH + MENU_TEXT_LOCATION);
	mHUD->loadData(FILE_PATH + MENU_TEXT_LOCATION);

	// Add sounds
	mSoundManager->loadSample(mDeathIndex, ASSET_PATH + SOUND_ASSET_PATH + mDeathSound);
	mSoundManager->loadSample(mGameOverIndex, ASSET_PATH + SOUND_ASSET_PATH + mGameOverSound);
	mSoundManager->loadSample(mChangeSpriteIndex, ASSET_PATH + SOUND_ASSET_PATH + mChangeSpriteSound);
	mSoundManager->loadSample(mSpawnIndex, ASSET_PATH + SOUND_ASSET_PATH + mSpawnSound);
	mIsInitted = true;

	// Adding to graphics buffer
	Color black(0, 0, 0, 255);
	Color gray(65, 65, 65, 255);
	Color white(255, 255, 255, 255);
	Color green(0, 255, 0, 255);
	GraphicsBuffer* pBlackBuffer = new GraphicsBuffer(DISP_WIDTH, DISP_HEIGHT);
	GraphicsBuffer* pTileBuffer = new GraphicsBuffer(gray, mTileSize, mTileSize);

	// Buffers for the lines separating each box
	GraphicsBuffer* pXSeparatorBuffer = new GraphicsBuffer(white, mGridSize * (mTileSize + TILE_PADDING) + TILE_PADDING, TILE_PADDING);
	GraphicsBuffer* pYSeparatorBuffer = new GraphicsBuffer(white, TILE_PADDING, mGridSize * (mTileSize + TILE_PADDING) + TILE_PADDING);

	// Buffers for a box highlighting the current tile clicked
	GraphicsBuffer* pXHighlightBuffer = new GraphicsBuffer(green, mTileSize + (TILE_PADDING * 2), TILE_PADDING);
	GraphicsBuffer* pYHighlightBuffer = new GraphicsBuffer(green, TILE_PADDING, mTileSize + (TILE_PADDING * 2));

	assert(pBlackBuffer && pTileBuffer && pXSeparatorBuffer && pYSeparatorBuffer && pXHighlightBuffer && pYHighlightBuffer);

	mGraphicsSystem->setBitmapToColor(*pBlackBuffer, black);

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

	// Init grid
	mGridManager->init(*mGraphicsBufferManager, mTileIndex);
	// Init HUD
	mHUD->init(*mGraphicsBufferManager, mTileIndex, mGridSize);
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
	
	// Basic values for now, will adjust once other features start working
	mGridManager->loadGrid(SUDOKU, mGridSize, DISP_WIDTH, DISP_HEIGHT);

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
			mGridManager->draw(mBoxSizeX, mBoxSizeY, mXSeparatorIndex, mYSeparatorIndex, mTileIndex, mXHighlightIndex, mYHighlightIndex);

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

	//if (gameEvent.getType() == START_SOUND_EVENT) { mSoundManager->soundOn(); }
	//if (gameEvent.getType() == STOP_SOUND_EVENT) { mSoundManager->soundOff(); }

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
