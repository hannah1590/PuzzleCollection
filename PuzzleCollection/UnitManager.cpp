#include "UnitManager.h"

UnitManager::UnitManager()
{
	mNumAllocatedObjects = 0; // 0 units to start
}

UnitManager::~UnitManager()
{
	clearUnits();
}

// Delete every unit still alive
void UnitManager::clearUnits()
{
	for (auto i : mUnits)
	{
		delete i;
		i = nullptr;
	}
	mUnits.clear();
}

// Calls each unit's update function and checks if they need to be deleted
void UnitManager::update(float frameRate)
{
	for (auto i : mUnits)
	{
		if (!i->getFree())
		{
			i->update(frameRate);
		}
	}
	deleteUnit();
}

// Draw each unit 
void UnitManager::draw(GraphicsSystem& pGraphicsSystem)
{
	for (auto i : mUnits)
	{
		i->draw(pGraphicsSystem);
	}
}

// Draw each unit as invisible - prevents errors
void UnitManager::drawInvisible(GraphicsSystem& pGraphicsSystem)
{
	for (auto i : mUnits)
	{
		i->drawInvisible(pGraphicsSystem);
	}
}

// Add a unit to the vector and allocate memory for it if there is room - prevents excessive units being made
void UnitManager::addUnit(Unit& newUnit)
{
	if (mNumAllocatedObjects < MAX_NUM_OBJECTS)
	{
		allocateObject(newUnit);
		mUnits.push_back(&newUnit);
	}
}

// Deletes and frees a unit if it is outside of the screen
void UnitManager::deleteUnit() 
{
	for (auto i : mUnits)
	{
		if (i->getLocation().getX() >= DISP_WIDTH || i->getLocation().getX() <= 0 || i->getLocation().getY() >= DISP_HEIGHT || i->getLocation().getY() <= 0)
		{
			if (!i->getAnimationChange())
			{
				//EventSystem* pEventSystem = EventSystem::getInstance();
				//GameEvent gameEvent(REMOVING_SCORE_EVENT);
				//pEventSystem->fireEvent(gameEvent);
			}
			freeObject(*i);
			mUnits.erase(remove(mUnits.begin(), mUnits.end(), i), mUnits.end());
			delete i;
			i = nullptr;
		}
	}
}

// Chages a specific unit's animation and adds to the score
void UnitManager::changeUnitAnimation(Unit& unit)
{
	if (!unit.getAnimationChange())
	{
		unit.setAnimation();
		unit.changeAnimation();

		//EventSystem* pEventSystem = EventSystem::getInstance();
		//GameEvent gameEvent(ADDING_SCORE_EVENT);
		//pEventSystem->fireEvent(gameEvent);
	}
}

// Get the unit at the specified location
Unit* UnitManager::getObjectAtLocation(Vector2D loc)
{
	for (auto i : mUnits)
	{
		if (i->getLocation().getX() + (i->getPixelWidth() / 2) >= loc.getX() && i->getLocation().getX() - (i->getPixelWidth() / 2) <= loc.getX())
		{
			if (i->getLocation().getY() + (i->getPixelHeight() / 2) >= loc.getY() && i->getLocation().getY() - (i->getPixelHeight() / 2) <= loc.getY())
			{
				return i;
			}
		}
	}
}

// Resets amount of units allocated
void UnitManager::resetObjectPool()
{
	mNumAllocatedObjects = 0;
}

// Allocates space for a unit
Unit* UnitManager::allocateObject(Unit& unit)
{
	mNumAllocatedObjects++;
	unit.free();
	return &unit;
}

void UnitManager::freeObject(Unit& ptr)
{
	// Make sure that the address passed in is actually one managed by this pool
	if (contains(ptr))
	{
		// Add address back to free list
		ptr.free();
		mNumAllocatedObjects--;
	}
	else
	{
		cout << "ERROR:  object freed from a pool that doesn't manage it\n";
	}
}

// Checks if unit is in the vector
bool UnitManager::contains(Unit& ptr) const
{
	for (auto i : mUnits)
	{
		if (i == &ptr)
		{
			return &ptr;
		}
	}
}