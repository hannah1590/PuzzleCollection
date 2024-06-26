#pragma once

#include "Trackable.h"
#include "Vector2D.h"

#include <vector>

/*
Dean Lawson
Champlain College
*/

class RayCaster :public Trackable
{
public:
	static std::vector<Vector2D> getPoints(const Vector2D& start, const Vector2D& end, float interval);
};