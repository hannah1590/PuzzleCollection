#include "GraphicsBufferManager.h"
GraphicsBufferManager::~GraphicsBufferManager()
{
	clear();
}

void GraphicsBufferManager::addBuffer(int index, GraphicsBuffer& newGraphicsBuffer)
{
	mGraphicsBuffers.emplace(index, &newGraphicsBuffer);
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

void GraphicsBufferManager::deleteBuffer(int index)
{
	delete mGraphicsBuffers[index];
	mGraphicsBuffers[index] = nullptr;
	mGraphicsBuffers.erase(index);
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