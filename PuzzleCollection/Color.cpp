#include "Color.h"

// Default equals white
Color::Color()
{
	mR = 255;
	mG = 255;
	mB = 255;
	mA = 255;
}

Color::Color(int r, int g, int b, int a) 
{
	mR = r;
	mG = g;
	mB = b;
	mA = a;
}

void Color::setValues(int r, int g, int b, int a)
{
	mR = r;
	mG = g;
	mB = b;
	mA = a;
}


bool Color::operator==(Color color)
{
	if (mR == color.getR() && mG == color.getG() && mB == color.getB() && mA == color.getA())
		return true;
	else
		return false;
}