#pragma once
#include "GraphicsBuffer.h"
#include <map>

// Stores all graphics buffers
class GraphicsBufferManager : public Trackable
{
public:
	GraphicsBufferManager() = default;
	~GraphicsBufferManager();

	void clear();

	void addBuffer(int index, GraphicsBuffer& newGraphicsBuffer) { mGraphicsBuffers.emplace(index, &newGraphicsBuffer); }
	GraphicsBuffer* getBuffer(int index) { return mGraphicsBuffers[index]; }
	void deleteBuffer(int index) { mGraphicsBuffers.erase(index); };
private:
	std::map<int, GraphicsBuffer*> mGraphicsBuffers;
};