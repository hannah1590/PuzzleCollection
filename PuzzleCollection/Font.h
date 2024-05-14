#pragma once
#include "AllegroInclude.h"

#include <string>
#include "GraphicsBuffer.h"

class Font:public Trackable
{
	friend class GraphicsSystem;
public:
	Font(const std::string& filename, int size);
	~Font() { al_destroy_font(mFont); };
	int getSize() { return mFontSize; }
private:
	ALLEGRO_FONT* mFont;
	int mFontSize;
};