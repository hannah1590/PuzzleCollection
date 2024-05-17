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
	mHUD = new HUD(*mGraphicsSystem);
	mSoundManager = new SoundManager(mMaxSamples);
	mMenuManager = new MenuManager(*mGraphicsSystem);
	mGridManager = new GridManager(9, 3, 3); // basic variables
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
	gameVariables.close();
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
	pEventSystem->addListener((EventType)START_GAME_EVENT, this);
	pEventSystem->addListener((EventType)PAUSING_GAME_EVENT, this);
	pEventSystem->addListener((EventType)RESUMING_GAME_EVENT, this);
	pEventSystem->addListener((EventType)SWITCHING_ANIMATIONS_EVENT, this);
	pEventSystem->addListener((EventType)QUITTING_EVENT, this);
	pEventSystem->addListener((EventType)ADDING_SCORE_EVENT, this);
	pEventSystem->addListener((EventType)REMOVING_SCORE_EVENT, this);

	// Add sounds
	mSoundManager->loadSample(mDeathIndex, ASSET_PATH + SOUND_ASSET_PATH + mDeathSound);
	mSoundManager->loadSample(mGameOverIndex, ASSET_PATH + SOUND_ASSET_PATH + mGameOverSound);
	mSoundManager->loadSample(mChangeSpriteIndex, ASSET_PATH + SOUND_ASSET_PATH + mChangeSpriteSound);
	mSoundManager->loadSample(mSpawnIndex, ASSET_PATH + SOUND_ASSET_PATH + mSpawnSound);
	mIsInitted = true;

	// TO DO: init graphics buffers here
}

// Delete game systems in opposite order of creation
void Game::cleanup()
{
	delete mGridManager;
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
	const double frameRate = 60.0;
	double targetTime = 1000.0 / frameRate;

	// Adding to graphics buffer
	Color black(0, 0, 0, 255);
	GraphicsBuffer* pBlackBuffer = new GraphicsBuffer(DISP_WIDTH, DISP_HEIGHT);
	GraphicsBuffer* pRedBallBuffer = new GraphicsBuffer(ASSET_PATH + GLOW_BALLS_FILENAME);
	GraphicsBuffer* pBlueBallBuffer = new GraphicsBuffer(ASSET_PATH + GLOW_BALLS_FILENAME);

	assert(pBlackBuffer && pRedBallBuffer && pBlueBallBuffer);

	mGraphicsSystem->setBitmapToColor(*pBlackBuffer, black);

	int bufferIndex = 0;

	// Add each graphics buffer to the manager
	mGraphicsBufferManager->addBuffer(bufferIndex, *pBlackBuffer); bufferIndex++;
	mGraphicsBufferManager->addBuffer(bufferIndex, *pRedBallBuffer); bufferIndex++;
	mGraphicsBufferManager->addBuffer(bufferIndex, *pBlueBallBuffer); bufferIndex++;

	srand((unsigned int)time(NULL));

	double frames = 0;
	double deltaTime = 0;

	int secondsPassed = 100;
	bool happened = false;
/*
	while (mIsLooping)
	{
		// Start loop timer
		pPerformanceTracker->clearTracker("loop");
		pPerformanceTracker->startTracking("loop");
		Timer timer;
		timer.start();
		
		if (!mMenuManager->getIsMenuOpen())
		{
			//If a menu is not open draw what is needed for the game
			mInputSystem->updateEvents();
			mGraphicsSystem->drawBackbuffer(Vector2D(0, 0), *mGraphicsBufferManager->getBuffer(0), 1.0);

			mHUD->update(mPoints, mSavedTime);
			mGraphicsSystem->flip();
		}

		if (mMenuManager->getIsMenuOpen())
		{
			//If a menu is open do not draw game but draw what is needed for the menu
			mHUD->pauseTimer();
			mInputSystem->updateEvents();
			mGraphicsSystem->drawBackbuffer(Vector2D(0, 0), *mGraphicsBufferManager->getBuffer(0), 1.0);
			mMenuManager->draw();

			mGraphicsSystem->flip();
		}

		timer.sleepUntilElapsed(targetTime);
		pPerformanceTracker->stopTracking("loop");
	}*/
	delete pPerformanceTracker;
}

// Recieves and applies events
void Game::handleEvent(const Event& theEvent)
{
	const GameEvent& gameEvent = static_cast<const GameEvent&>(theEvent);

	if (gameEvent.getType() == START_GAME_EVENT)
	{
		mHasStarted = true;
	}
	if (gameEvent.getType() == PAUSING_GAME_EVENT)
	{
		mHasPaused = true;
	}
	if (gameEvent.getType() == RESUMING_GAME_EVENT)
	{
		mHasPaused = false;
	}
	if (gameEvent.getType() == SWITCHING_ANIMATIONS_EVENT)
	{
		mUnitManager->changeUnitAnimation(*mUnitManager->getObjectAtLocation(gameEvent.getMouseLocation()));
	}
	if (gameEvent.getType() == QUITTING_EVENT)
	{
		mIsLooping = false;
		mHasPaused = false;
	}
	if (gameEvent.getType() == ADDING_SCORE_EVENT)
	{
		mPoints += mPointIncrease;
		mSoundManager->playSample(false, mChangeSpriteIndex);
	}
	if (gameEvent.getType() == REMOVING_SCORE_EVENT)
	{
		mPoints -= mPointDecrease;
		if (mPoints < 0)
		{
			mPoints = 0;
			mSoundManager->playSample(false, mGameOverIndex);
		}
		mSoundManager->playSample(false, mDeathIndex);
	}
}

// Creates a sphere at a random speed, velocity, and spawn location based on game variables
void Game::createRandomUnit()
{
	if (mUnitManager->getMaxObjectects() > mUnitManager->getAllocatedObjects()) 
	{
		int range = (mMaxVelocity - mMinVelocity) + 1;
		int x = rand() % range + mMinVelocity;
		int y = rand() % range + mMinVelocity;
		Vector2D vel(x, y);
		range = (mMaxSpeed - mMinSpeed) + 1;
		int speed = rand() % range + mMinSpeed;

		int spawnRangeX = rand() % (((DISP_WIDTH / 2) + mSpawnRadius) - ((DISP_WIDTH / 2) - mSpawnRadius)) + ((DISP_WIDTH / 2) - mSpawnRadius);
		int spawnRangeY = rand() % (((DISP_HEIGHT / 2) + mSpawnRadius) - ((DISP_HEIGHT / 2) - mSpawnRadius)) + ((DISP_HEIGHT / 2) - mSpawnRadius);

		Unit* pUnit = new Unit(*mGraphicsBufferManager, Vector2D(spawnRangeX, spawnRangeY), vel, speed);
		mUnitManager->addUnit(*pUnit);

		mSoundManager->playSample(false, mSpawnIndex);
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
