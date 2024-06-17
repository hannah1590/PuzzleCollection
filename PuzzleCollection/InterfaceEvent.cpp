#include "InterfaceEvent.h"

// InterfaceEvent used the Event class as a basis so it can be used in conjuction with the EventSystem/EventListener
InterfaceEvent::InterfaceEvent(InterfaceEventType type, int inputKey)
	:Event((EventType)type)
{
	mInterfaceType = type;
	mInputKey = inputKey;
}

InterfaceEvent::~InterfaceEvent()
{
}