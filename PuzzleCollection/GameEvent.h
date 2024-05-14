#pragma once
#include <Event.h>
#include "InterfaceEvent.h"

enum GameEventType
{
	START_GAME_EVENT = 2,
	PAUSING_GAME_EVENT,
	RESUMING_GAME_EVENT,
	SWITCHING_ANIMATIONS_EVENT,
	QUITTING_EVENT,
	ADDING_SCORE_EVENT,
	REMOVING_SCORE_EVENT,
	NUM_GAME_EVENT_TYPES
};

class GameEvent : public Event
{
public:
	GameEvent(GameEventType type);
	virtual ~GameEvent();

	Vector2D getMouseLocation() const { return mMouseLocation; }
	void setMouseLocation(Vector2D mouseLocation) { mMouseLocation = mouseLocation; }

	GameEventType getType() const { return mGameType; }
private:
	Vector2D mMouseLocation;
	GameEventType mGameType;
};