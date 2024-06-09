#pragma once
#include "AllegroInclude.h"

#include <string>
#include "GraphicsBuffer.h"

// Holds all font information
class Font : public Trackable
{
	friend class GraphicsSystem;
public:
	Font() = default;
	Font(const std::string& filename, int size);
	~Font() { al_destroy_font(mFont); };
	int getSize() { return mFontSize; }
private:
	ALLEGRO_FONT* mFont;
	int mFontSize;
};