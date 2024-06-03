#include "InputTranslator.h"

InputTranslator::InputTranslator() 
{
	EventSystem* pEventSystem = EventSystem::getInstance();
	pEventSystem->addListener((EventType)MOUSE_CLICK_EVENT, this);
	pEventSystem->addListener((EventType)KEYBOARD_EVENT, this);

	mGameStarted = false;
	mGamePaused = false;
}

InputTranslator::~InputTranslator()
{
}

// Checks the Interface event type and fires a GameEvent based on the current keyboard or mouse click
void InputTranslator::handleEvent(const Event& theEvent)
{
	EventSystem* pEventSystem = EventSystem::getInstance();
	const InterfaceEvent& interfaceEvent = static_cast<const InterfaceEvent&>(theEvent);
	if (theEvent.getType() == MOUSE_CLICK_EVENT)
	{
		if (interfaceEvent.getInputKey() == LEFT_MOUSE)
		{
			// Left mouse click progresses game
			GameEvent gameEvent(SCREEN_CLICK_EVENT);
			gameEvent.setMouseLocation(interfaceEvent.getMouseLocation());
			pEventSystem->fireEvent(gameEvent);
		}
	}
	else if (theEvent.getType() == KEYBOARD_EVENT)
	{
		// Space bar starts/pauses game
		if (interfaceEvent.getInputKey() == SPACE)
		{
			GameEvent gameEvent(START_STOP_EVENT);
			pEventSystem->fireEvent(gameEvent);
		}
		// Escape key quits game 
		if (interfaceEvent.getInputKey() == ESCAPE)
		{
			GameEvent gameEvent(QUITTING_EVENT);
			pEventSystem->fireEvent(gameEvent);
		}
	}
}