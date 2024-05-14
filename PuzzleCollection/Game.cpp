#include "Game.h"

Game* Game::mspInstance = nullptr;

Game::Game()
{
	getVariables();
	getSounds();

	mInputSystem = new InputSystem;
	mGraphicsSystem = new GraphicsSystem;
	mGraphicsBufferManager = new GraphicsBufferManager;
	mUnitManager = new UnitManager;
	mInputTranslator = new InputTranslator;
	mHUD = new HUD(*mGraphicsSystem);
	mSoundManager = new SoundManager(mMaxSamples);
}

Game::~Game()
{
	cleanup();
}

void Game::getVariables()
{
	ifstream gameVariables(FILE_PATH + GAME_VARIABLES_FILENAME);
	if (!gameVariables.is_open())
	{
		cout << "Error getting file";
	}
	gameVariables >> mMinVelocity >> mMaxVelocity >>
		mMinSpeed >> mMaxSpeed >>
		mMaxSpeedCap >> mMinSpeedCap >>
		mSpawnRadius >> mSpawnRadiusCap >>
		mRandomChance >> mRandomChanceCap >>
		mSecondsUntilIncrease >> mSpeedIncreaseRate >>
		mSpawnIncreaseRate >> mRandomChanceIncreaseRate >>
		mPointIncrease >> mPointDecrease;
	gameVariables.close();
}

void Game::getSounds()
{
	ifstream soundFiles(FILE_PATH + SOUND_FILENAMES_LOCATION);
	if (!soundFiles.is_open())
	{
		cout << "Error getting file";
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

	if (!mSoundManager->init())
	{
		cout << "ERROR - Sound init failed\n";
	}

	mGraphicsSystem->createDisplay(DISP_WIDTH, DISP_HEIGHT);

	if (mIsInitted)
	{
		cleanup();
	}

	// add game events
	EventSystem* pEventSystem = EventSystem::getInstance();
	pEventSystem->addListener((EventType)START_GAME_EVENT, this);
	pEventSystem->addListener((EventType)PAUSING_GAME_EVENT, this);
	pEventSystem->addListener((EventType)RESUMING_GAME_EVENT, this);
	pEventSystem->addListener((EventType)SWITCHING_ANIMATIONS_EVENT, this);
	pEventSystem->addListener((EventType)QUITTING_EVENT, this);
	pEventSystem->addListener((EventType)ADDING_SCORE_EVENT, this);
	pEventSystem->addListener((EventType)REMOVING_SCORE_EVENT, this);

	// add sounds
	mSoundManager->loadSample(mDeathIndex, ASSET_PATH + SOUND_ASSET_PATH + mDeathSound);
	mSoundManager->loadSample(mGameOverIndex, ASSET_PATH + SOUND_ASSET_PATH + mGameOverSound);
	mSoundManager->loadSample(mChangeSpriteIndex, ASSET_PATH + SOUND_ASSET_PATH + mChangeSpriteSound);
	mSoundManager->loadSample(mSpawnIndex, ASSET_PATH + SOUND_ASSET_PATH + mSpawnSound);
	mIsInitted = true;
}

void Game::cleanup()
{
	delete mSoundManager;
	delete mHUD;
	delete mInputTranslator;
	delete mUnitManager;
	delete mGraphicsBufferManager;
	delete mGraphicsSystem;
	delete mInputSystem;
	mIsInitted = false;
}

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

	mGraphicsBufferManager->addBuffer(bufferIndex, *pBlackBuffer); bufferIndex++;
	mGraphicsBufferManager->addBuffer(bufferIndex, *pRedBallBuffer); bufferIndex++;
	mGraphicsBufferManager->addBuffer(bufferIndex, *pBlueBallBuffer); bufferIndex++;
	srand((unsigned int)time(NULL));

	double frames = 0;
	double deltaTime = 0;

	int secondsPassed = 100;
	bool happened = false;

	while (mIsLooping)
	{
		pPerformanceTracker->clearTracker("loop");
		pPerformanceTracker->startTracking("loop");
		Timer timer;
		timer.start();

		// loads start screen
		while (!mHasStarted)
		{
			timer.pause(true);
			Timer start;
			start.start();

			loadScreen(targetTime);
			mHUD->welcomeScreen();

			mGraphicsSystem->flip();

			start.sleepUntilElapsed(targetTime);
			mTimePaused += start.getElapsedTime();
			mTimeElapsed += start.getElapsedTime();
			timer.start();
		}

		// loads pause screen
		while (mHasPaused)
		{
			timer.pause(true);
			Timer paused;
			paused.start();

			loadScreen(targetTime);
			mHUD->pauseScreen();

			mGraphicsSystem->flip();

			paused.sleepUntilElapsed(targetTime);
			mTimePaused += paused.getElapsedTime();
			mTimeElapsed += paused.getElapsedTime();
			timer.start();
		}

		// loads game over screen
		if (mPoints <= 0)
		{
			while (mIsLooping)
			{
				loadScreen(targetTime);
				mHUD->gameOver();

				mGraphicsSystem->flip();
			}
		}

		loadScreen(targetTime);
		
		int random = rand() % mRandomChance + 1;
		if (random == 1)
		{
			createRandomUnit();
		}

		mHUD->fps((frames / deltaTime) * 1000);
		mGraphicsSystem->flip();

		// increases variables if a certain number of seconds has passed
		if (secondsPassed % mSecondsUntilIncrease == 0 && !happened)
		{
			mMinSpeed += mSpeedIncreaseRate;
			mMaxSpeed += mSpeedIncreaseRate;
			mSpawnRadius += mSpawnIncreaseRate;
			mRandomChance -= mRandomChanceIncreaseRate;
			happened = true;

			if (mMaxSpeed > mMaxSpeedCap)
			{
				mMaxSpeed = mMaxSpeedCap;
			}
			if (mMinSpeed > mMinSpeedCap)
			{
				mMinSpeed = mMinSpeedCap;
			}
			if (mSpawnRadius > mSpawnRadiusCap)
			{
				mSpawnRadius = mSpawnRadiusCap;
			}
			if (mRandomChance < mRandomChanceCap)
			{
				mRandomChance = mRandomChanceCap;
			}
		}

		timer.sleepUntilElapsed(targetTime);
		pPerformanceTracker->stopTracking("loop");

		mTimeElapsed += timer.getElapsedTime();
		if ((int)mTimeElapsed / 1000 > secondsPassed) // makes sure data driven variables increase only once a second
		{
			happened = false;
		}
		secondsPassed = mTimeElapsed / 1000;

		// calculate frames and deltatime
		frames++;
		deltaTime += timer.getElapsedTime();
		if (frames == frameRate)
		{
			frames = 0;
			deltaTime = 0;
		}
	}
	delete pPerformanceTracker;
}

void Game::loadScreen(double targetTime)
{
	mInputSystem->updateEvents();
	mGraphicsSystem->drawBackbuffer(Vector2D(0, 0), *mGraphicsBufferManager->getBuffer(0), 1.0);

	if (mHasPaused || mPoints <= 0)
	{
		mUnitManager->drawInvisible(*mGraphicsSystem);
	}
	else 
	{
		mUnitManager->update(targetTime);
		mUnitManager->draw(*mGraphicsSystem);
	}

	mHUD->score(mPoints);
	mHUD->timer((mTimeElapsed - mTimePaused) / 1000);
}

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
