#pragma once
#include <Trackable.h>
#include <EventListener.h>
#include <EventSystem.h>
#include "InterfaceEvent.h"
#include "GameEvent.h"

/* Translates InterfaceEvents into GameEvents
   by Hannah Fasco
*/

class InputTranslator : public EventListener
{
public:
	InputTranslator();
	virtual ~InputTranslator();

	void handleEvent(const Event& theEvent);

private:
	bool mGameStarted;  // Whether the game has started or not
	bool mGamePaused;   // If the game is currently paused
};