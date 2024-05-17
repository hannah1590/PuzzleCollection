#pragma once
#include <Vector2D.h>
#include <string>

#include "GraphicsBuffer.h"
#include "Sprite.h"
#include "Font.h"
#include "Color.h"
using namespace std;

// Handles allegro draw functions and buffers
class GraphicsSystem:public Trackable
{
public:
	GraphicsSystem();
	~GraphicsSystem();
	void createDisplay(int width, int height);

	bool initGraphics();
	void cleanup();

	GraphicsBuffer* getBackbuffer(GraphicsBuffer& pGraphicsBuffer);
	Vector2D getSize(GraphicsBuffer& graphicsBuffer);
	void flip();

	void drawBackbuffer(const Vector2D& targetLoc, Sprite& sprite, float scale); 
	void drawGraphicsBuffer(GraphicsBuffer& targetGraphicsBuffer, const Vector2D& targetLoc, Sprite& sprite, float scale);

	void drawBackbuffer(const Vector2D& targetLoc, GraphicsBuffer& destination, float scale); 
	void drawGraphicsBuffer(GraphicsBuffer& targetGraphicsBuffer, const Vector2D& targetLoc, GraphicsBuffer& destination, float scale); 

	void writeTextToBackbuffer(const Vector2D& targetLoc, Font& font, Color& color, const std::string& text);
	void writeTextToGraphicsBuffer(GraphicsBuffer& targetGraphicsBuffer, const Vector2D& targetLoc, Font& font,  Color& color, const std::string& text);
	void saveBuffer(GraphicsBuffer& buffer, const std::string& filename);

	void setBitmapToColor(GraphicsBuffer& bitmap, Color& color);

	// By Nate
	void drawButtons(int amount, float spacing, float size, float x1, float y1, Color& color);
private:
	ALLEGRO_DISPLAY* mDisplay;
};