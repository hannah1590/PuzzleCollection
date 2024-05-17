#pragma once
#include "GraphicsBuffer.h"

// Stores color variables to limit calls to allegro
class Color : public Trackable
{
public:
	Color(int r, int g, int b, int a);
	int getR() { return mR; }
	int getG() { return mG; }
	int getB() { return mB; }
	int getA() { return mA; }
private:
	int mR;
	int mG;
	int mB;
	int mA;
};