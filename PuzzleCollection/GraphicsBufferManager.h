#pragma once
#include "GraphicsBuffer.h"
#include <map>

/* Stores all graphics buffers
   By Hannah Fasco
*/

class GraphicsBufferManager : public Trackable
{
public:
	GraphicsBufferManager() = default;
	~GraphicsBufferManager();

	void clear();

	void addBuffer(int index, GraphicsBuffer& newGraphicsBuffer);
	void deleteBuffer(int index);

	bool doesExist(int index);

	GraphicsBuffer* getBuffer(int index) { return mGraphicsBuffers[index]; }
	int getSize() { return mGraphicsBuffers.size(); }
private:
	std::map<int, GraphicsBuffer*> mGraphicsBuffers;   // Map of all current graphics buffers in use
};