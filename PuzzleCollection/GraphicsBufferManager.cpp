#include "GraphicsBufferManager.h"
GraphicsBufferManager::~GraphicsBufferManager()
{
	clear();
}

void GraphicsBufferManager::clear()
{
	for (auto i : mGraphicsBuffers)
	{
		delete(i.second);
		i.second = nullptr;
	}
	mGraphicsBuffers.clear();
}