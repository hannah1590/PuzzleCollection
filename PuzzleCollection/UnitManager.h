#pragma once
#include "Unit.h"
#include "GameEvent.h"
#include "EventSystem.h"

class UnitManager:public Trackable
{
public:
	UnitManager();
	~UnitManager();
	void clearUnits();
	void update(float frameRate);
	void draw(GraphicsSystem& pGraphicsSystem);

	void drawInvisible(GraphicsSystem& pGraphicsSystem);

	void addUnit(Unit& newUnit);// { mUnits.push_back(&newUnit); }
	void deleteUnit();
	void changeUnitAnimation(Unit& unit);

	bool isUnitVectorEmpty() { return mUnits.empty(); }
	void isAnimating() { mIsAnimating = !mIsAnimating; }

	Unit* getObjectAtLocation(Vector2D loc);

	void resetObjectPool();
	Unit* allocateObject(Unit& unit);
	void freeObject(Unit& ptr);

	unsigned int getNumFreeObjects() { return MAX_NUM_OBJECTS - mNumAllocatedObjects; };
	
	bool contains(Unit& ptr) const;

	int getAllocatedObjects() { return mNumAllocatedObjects; }
	int getMaxObjectects() { return MAX_NUM_OBJECTS; }
private:
	const int DISP_WIDTH = 800;
	const int DISP_HEIGHT = 600;

	const unsigned int MAX_NUM_OBJECTS = 50;
	
	vector<Unit*> mUnits;
	bool mIsAnimating = true;

	unsigned int mNumAllocatedObjects;
};