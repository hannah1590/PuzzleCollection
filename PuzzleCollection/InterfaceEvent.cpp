#include "InterfaceEvent.h"

InterfaceEvent::InterfaceEvent(InterfaceEventType type, int inputKey)
	:Event((EventType)type)
{
	mInterfaceType = type;
	mInputKey = inputKey;
}

InterfaceEvent::~InterfaceEvent()
{
}