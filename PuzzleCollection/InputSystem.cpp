#include "InputSystem.h"

InputSystem::InputSystem()
{
}

InputSystem::~InputSystem()
{
	cleanup();
}

bool InputSystem::init()
{
	bool initKeyboard;
	bool initMouse;

	if (!al_init()) 
	{
		return false;
	}

	if (!al_install_keyboard())
	{
		cout << "ERROR - Keyboard not installed\n";
		return false;
	}
	if (!al_install_mouse())
	{
		cout << "ERROR - Mouse not installed\n";
		return false;
	}

	mEventQueue = al_create_event_queue();
	if (mEventQueue == nullptr)
	{
		cout << "ERROR - Event Queue not created\n";
		return false;
	}

	al_register_event_source(mEventQueue, al_get_mouse_event_source());
	al_register_event_source(mEventQueue, al_get_keyboard_event_source());
	return true;
}

void InputSystem::cleanup()
{
	al_uninstall_keyboard();
	al_uninstall_mouse();
}

bool InputSystem::getKeyState(InputKeys keyCheck)
{
	ALLEGRO_KEYBOARD_STATE state;
	al_get_keyboard_state(&state);
	return al_key_down(&state, keyCheck);
}

bool InputSystem::getMouseState(InputKeys mouseCheck)
{
	ALLEGRO_MOUSE_STATE state;
	al_get_mouse_state(&state);
	if (state.buttons & mouseCheck) return true;
	else return false;
}

Vector2D InputSystem::getMouseLocation()
{
	ALLEGRO_MOUSE_STATE state;
	al_get_mouse_state(&state);
	return Vector2D(state.x, state.y);
}

void InputSystem::updateEvents()
{
	EventSystem* pEventSystem = EventSystem::getInstance();

	while (!al_is_event_queue_empty(mEventQueue)) 
	{
		al_get_next_event(mEventQueue, &mCurrentEvent);
		if (mCurrentEvent.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) 
		{
			InterfaceEvent interfaceEvent(MOUSE_CLICK_EVENT, mCurrentEvent.mouse.button);
			interfaceEvent.setMouseLocation(getMouseLocation());
			pEventSystem->fireEvent(interfaceEvent);
		}
		if (mCurrentEvent.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			InterfaceEvent interfaceEvent(KEYBOARD_EVENT, mCurrentEvent.keyboard.keycode);
			pEventSystem->fireEvent(interfaceEvent);
		}
	}
}