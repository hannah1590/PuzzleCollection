#include "GameEvent.h"

GameEvent::GameEvent(GameEventType type)
	:Event((EventType)type)
{
	mGameType = type;
}

GameEvent::~GameEvent()
{
}