#pragma once
#include <Event.h>
#include "InterfaceEvent.h"

enum GameEventType
{
	START_STOP_EVENT = 2,
	NEW_GAME_EVENT,
	CHANGE_DIFFICULTY_EVENT,
	QUITTING_EVENT,
	SCREEN_CLICK_EVENT,
	STOP_SOUND_EVENT,
	START_SOUND_EVENT,
	OPEN_CLOSE_NOTES_EVENT,
	NUM_GAME_EVENT_TYPES
};

// Stores all game events
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