#pragma once
#include "GraphicsSystem.h"
#include "InterfaceEvent.h"
#include <EventSystem.h>

/* Controls allegro keyboard and mouse 
   by Hannah Fasco
*/

// All current supported inputs
enum InputKeys
{
	LEFT_MOUSE = 1, // Click on an orb to change animation
	RIGHT_MOUSE = 2, 
	F_KEY = ALLEGRO_KEY_F, 
	S_KEY = ALLEGRO_KEY_S, 
	SPACE = ALLEGRO_KEY_SPACE, // Start application; unpause
	ENTER = ALLEGRO_KEY_ENTER, 
	ESCAPE = ALLEGRO_KEY_ESCAPE // Pause screen; exit application when paused
};

class InputSystem : public Trackable
{
public:
	InputSystem() = default;
	~InputSystem();

	bool init();
	void cleanup();

	bool getKeyState(InputKeys keyCheck); 
	bool getMouseState(InputKeys mouseCheck);
	Vector2D getMouseLocation();

	void updateEvents(); // fire interface events

private:
	ALLEGRO_EVENT_QUEUE* mEventQueue;
	ALLEGRO_EVENT mCurrentEvent;
};