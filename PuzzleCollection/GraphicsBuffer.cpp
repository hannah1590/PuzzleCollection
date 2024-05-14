#include "GraphicsBuffer.h"
GraphicsBuffer::GraphicsBuffer(int width, int height) 
{
	mBitmap = al_create_bitmap(width, height);
}

GraphicsBuffer::GraphicsBuffer(const std::string& filename) 
{
	mBitmap = al_load_bitmap((filename).c_str());
}

GraphicsBuffer::~GraphicsBuffer() 
{
	if (mBitmap != al_get_target_bitmap()) 
	{ 
		al_destroy_bitmap(mBitmap); 
	}
}