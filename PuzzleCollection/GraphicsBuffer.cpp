#include "GraphicsBuffer.h"
#include "Color.h"

// Creates a bitmap that is just a single color
GraphicsBuffer::GraphicsBuffer(Color& color, int width, int height)
{
	ALLEGRO_BITMAP* pOldTarget = al_get_target_bitmap();
	const ALLEGRO_COLOR pColor = al_map_rgba(color.getR(), color.getG(), color.getB(), color.getA());

	mBitmap = al_create_bitmap(width, height);
	al_set_target_bitmap(mBitmap);
	al_clear_to_color(pColor);

	al_set_target_bitmap(pOldTarget);
}

// Creates a bitmap with a simple width and height
GraphicsBuffer::GraphicsBuffer(int width, int height) 
{
	mBitmap = al_create_bitmap(width, height);
}

// Creates a bitmap of an image from a file
GraphicsBuffer::GraphicsBuffer(const std::string& filename) 
{
	mBitmap = al_load_bitmap((filename).c_str());
}

// Cleans up bitmap
GraphicsBuffer::~GraphicsBuffer() 
{
	if (mBitmap != al_get_target_bitmap()) 
	{ 
		al_destroy_bitmap(mBitmap); 
	}
}