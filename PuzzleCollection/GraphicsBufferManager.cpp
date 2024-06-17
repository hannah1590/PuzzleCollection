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

// Adds a buffer to the map
void GraphicsBufferManager::addBuffer(int index, GraphicsBuffer& newGraphicsBuffer)
{
	mGraphicsBuffers.emplace(index, &newGraphicsBuffer);
}

// Deletes specified buffer from the map
void GraphicsBufferManager::deleteBuffer(int index)
{
	delete mGraphicsBuffers[index];
	mGraphicsBuffers[index] = nullptr;
	mGraphicsBuffers.erase(index);
}

// Checks if the buffer actually exists
bool GraphicsBufferManager::doesExist(int index)
{
	auto it = mGraphicsBuffers.find(index);
	if (it != mGraphicsBuffers.end())
	{
		return true;
	}
	return false;
}