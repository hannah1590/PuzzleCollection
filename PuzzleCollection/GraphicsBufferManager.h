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

	void addBuffer(int index, GraphicsBuffer& newGraphicsBuffer);
	GraphicsBuffer* getBuffer(int index) { return mGraphicsBuffers[index]; }
	bool doesExist(int index);
	void deleteBuffer(int index);
	int getSize() { return mGraphicsBuffers.size(); }
private:
	std::map<int, GraphicsBuffer*> mGraphicsBuffers;
};