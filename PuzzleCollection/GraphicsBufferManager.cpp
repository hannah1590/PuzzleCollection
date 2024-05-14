#include "GraphicsBufferManager.h"
GraphicsBufferManager::~GraphicsBufferManager()
{
	clear();
}

// Cleans map and deletes GraphicsBuffer pointers
void GraphicsBufferManager::clear()
{
	for (auto i : mGraphicsBuffers)
	{
		delete(i.second);
		i.second = nullptr;
	}
	mGraphicsBuffers.clear();
}