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
			GameEvent gameEvent(SWITCHING_ANIMATIONS_EVENT);
			gameEvent.setMouseLocation(interfaceEvent.getMouseLocation());
			pEventSystem->fireEvent(gameEvent);
		}
	}
	else if (theEvent.getType() == KEYBOARD_EVENT)
	{
		// Space bar starts game when on the start screen
		if (interfaceEvent.getInputKey() == SPACE && !mGameStarted)
		{
			mGameStarted = true;
			GameEvent gameEvent(START_GAME_EVENT);
			pEventSystem->fireEvent(gameEvent);
		}
		// Space bar resumes game when on the pause screen
		else if (interfaceEvent.getInputKey() == SPACE && mGameStarted && mGamePaused)
		{
			mGamePaused = false;
			GameEvent gameEvent(RESUMING_GAME_EVENT);
			pEventSystem->fireEvent(gameEvent);
		}
		// Escape key pauses game 
		if (interfaceEvent.getInputKey() == ESCAPE && !mGamePaused && mGameStarted)
		{
			mGamePaused = true;
			GameEvent gameEvent(PAUSING_GAME_EVENT);
			pEventSystem->fireEvent(gameEvent);
		}
		// Escape key exits game when on the pause screen
		else if (interfaceEvent.getInputKey() == ESCAPE && mGamePaused && mGameStarted)
		{
			GameEvent gameEvent(QUITTING_EVENT);
			pEventSystem->fireEvent(gameEvent);
		}
	}
}