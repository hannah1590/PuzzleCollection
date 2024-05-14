#pragma once
#include <Trackable.h>
#include <EventListener.h>
#include <EventSystem.h>
#include "InterfaceEvent.h"
#include "GameEvent.h"

class InputTranslator: public EventListener
{
public:
	InputTranslator();
	virtual ~InputTranslator();

	void handleEvent(const Event& theEvent);

private:
	bool mGameStarted;
	bool mGamePaused;
};