#pragma once
#include <Event.h>
#include <EventSystem.h>
#include "Game.h"

enum InterfaceEventType 
{
	MOUSE_CLICK_EVENT = NUM_BASE_EVENT_TYPES,
	KEYBOARD_EVENT,
	NUM_INTERFACE_EVENT_TYPES
};

// Stores all interface events
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
	int mInputKey;
	Vector2D mMouseLocation;
	InterfaceEventType mInterfaceType;
};

