#pragma once
#include <Event.h>
#include <EventSystem.h>
#include "Game.h"

/* Stores all interface events
   by Hannah Fasco
*/

// Different types of interface events
enum InterfaceEventType 
{
	MOUSE_CLICK_EVENT = NUM_BASE_EVENT_TYPES,
	KEYBOARD_EVENT,
	NUM_INTERFACE_EVENT_TYPES
};

class InterfaceEvent : public Event
{
public:
	InterfaceEvent(InterfaceEventType type, int inputKey);
	virtual ~InterfaceEvent();

	void setInputKey (int inputKey) { mInputKey = inputKey; }
	int getInputKey() const { return mInputKey; }

	void setMouseLocation(Vector2D mouseLocation) { mMouseLocation = mouseLocation; }
	Vector2D getMouseLocation() const { return mMouseLocation; }

	InterfaceEventType getType() const { return mInterfaceType; }
private:
	int mInputKey;                      // Current input used; all options found in InputSystem.h
	Vector2D mMouseLocation;            // Location of mouse on screen
	InterfaceEventType mInterfaceType;  // Type of event that just occured
};

