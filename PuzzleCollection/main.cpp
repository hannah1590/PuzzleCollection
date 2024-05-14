#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

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
	EventSystem::initInstance();
	EventSystem* pEventSystem = EventSystem::getInstance();
	pEventSystem->init();

	Game::initInstance();
	Game* pGame = Game::getInstance();
	pGame->init();

	pGame->doLoop();
	
	Game::cleanupInstance();
	EventSystem::cleanupInstance();

	MemoryTracker::getInstance()->reportAllocations(cout);

	system("pause");
	return 0;
}