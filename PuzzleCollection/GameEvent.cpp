#include "GameEvent.h"

// GameEvent used the Event class as a basis so it can be used in conjuction with the EventSystem/EventListener
GameEvent::GameEvent(GameEventType type)
	:Event((EventType)type)
{
	mGameType = type;
}

// Nothing to clean up
GameEvent::~GameEvent()
{
}