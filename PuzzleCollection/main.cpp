#include "AllegroInclude.h"
#include <iostream>
#include <cassert>
#include <string>

#include <PerformanceTracker.h>
#include <MemoryTracker.h>
#include <Timer.h>

#include <random>

#include "Game.h"
#include <EventSystem.h>

int main()
{
	// Inits singular event system
	EventSystem::initInstance();
	EventSystem* pEventSystem = EventSystem::getInstance();
	pEventSystem->init();

	// Inits singular game
	Game::initInstance();
	Game* pGame = Game::getInstance();
	pGame->init();

	// Plays game
	pGame->doLoop();
	
	// Cleans up game and event system
	Game::cleanupInstance();
	EventSystem::cleanupInstance();

	// Checks for memory leaks
	MemoryTracker::getInstance()->reportAllocations(cout);

	system("pause");
	return 0;
}