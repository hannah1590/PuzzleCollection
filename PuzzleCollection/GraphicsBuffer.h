#pragma once
#include "AllegroInclude.h"
#include <Vector2D.h>

class Color;

// Handles allegro bitmaps
class GraphicsBuffer : public Trackable
{
	friend class GraphicsSystem;
public:
	GraphicsBuffer(Color& color, int width, int height);
	GraphicsBuffer(int width, int height);
	GraphicsBuffer(const std::string& filename);
	~GraphicsBuffer();
	Vector2D getSize() { return Vector2D(al_get_bitmap_width(mBitmap), al_get_bitmap_height(mBitmap)); }
private:
	ALLEGRO_BITMAP* mBitmap;
};