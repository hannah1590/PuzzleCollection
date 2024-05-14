#include "UnitManager.h"
UnitManager::UnitManager()
{
	mNumAllocatedObjects = 0;
}

UnitManager::~UnitManager()
{
	clearUnits();
}

void UnitManager::clearUnits()
{
	for (auto i : mUnits)
	{
		delete i;
		i = nullptr;
	}
	mUnits.clear();
}

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

void UnitManager::draw(GraphicsSystem& pGraphicsSystem)
{
	for (auto i : mUnits)
	{
		i->draw(pGraphicsSystem);
	}
}

void UnitManager::drawInvisible(GraphicsSystem& pGraphicsSystem)
{
	for (auto i : mUnits)
	{
		i->drawInvisible(pGraphicsSystem);
	}
}

void UnitManager::addUnit(Unit& newUnit)
{
	if (mNumAllocatedObjects < MAX_NUM_OBJECTS)
	{
		allocateObject(newUnit);
		mUnits.push_back(&newUnit);
	}
}

void UnitManager::deleteUnit() 
{
	for (auto i : mUnits)
	{
		if (i->getLocation().getX() >= DISP_WIDTH || i->getLocation().getX() <= 0 || i->getLocation().getY() >= DISP_HEIGHT || i->getLocation().getY() <= 0)
		{
			if (!i->getAnimationChange())
			{
				EventSystem* pEventSystem = EventSystem::getInstance();
				GameEvent gameEvent(REMOVING_SCORE_EVENT);
				pEventSystem->fireEvent(gameEvent);
			}
			freeObject(*i);
			mUnits.erase(remove(mUnits.begin(), mUnits.end(), i), mUnits.end());
			delete i;
			i = nullptr;
		}
	}
}

void UnitManager::changeUnitAnimation(Unit& unit)
{
	if (!unit.getAnimationChange())
	{
		unit.setAnimation();
		unit.changeAnimation();

		EventSystem* pEventSystem = EventSystem::getInstance();
		GameEvent gameEvent(ADDING_SCORE_EVENT);
		pEventSystem->fireEvent(gameEvent);
	}
}

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

void UnitManager::resetObjectPool()
{
	mNumAllocatedObjects = 0;
}

Unit* UnitManager::allocateObject(Unit& unit)
{
	mNumAllocatedObjects++;
	unit.free();
	return &unit;
}

void UnitManager::freeObject(Unit& ptr)
{
	//make sure that the address passed in is actually one managed by this pool
	if (contains(ptr))
	{
		//add address back to free list
		ptr.free();
		mNumAllocatedObjects--;
	}
	else
	{
		cout << "ERROR:  object freed from a pool that doesn't manage it\n";
	}
}

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