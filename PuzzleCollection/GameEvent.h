#pragma once
#include <Event.h>
#include "InterfaceEvent.h"

/* Stores all game events
   Class base and puzzle additions by Hannah Fasco
   Contributions to UI and game state events done by Nate Spielman
*/

// All kinds of game events that can occur
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
	CHANGE_GRID_SIZE_EVENT,
	ADD_SCORE_EVENT,
	MINUS_SCORE_EVENT,
	WIN_GAME_EVENT,
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
	Vector2D mMouseLocation;   // Current location of the mouse on the screen
	GameEventType mGameType;   // Type of event that just occured
};