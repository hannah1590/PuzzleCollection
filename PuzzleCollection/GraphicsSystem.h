#pragma once
#include <Vector2D.h>
#include <string>

#include "GraphicsBuffer.h"
#include "Sprite.h"
#include "Font.h"
#include "Color.h"
using namespace std;

class GraphicsBuffer;

/* Handles allegro draw functionsand buffers
   by Hannah Fasco
   drawButtons function by Nate Spielman
*/

class GraphicsSystem:public Trackable
{
public:
	GraphicsSystem();
	~GraphicsSystem();
	void createDisplay(int width, int height);

	bool initGraphics();
	void cleanup();

	GraphicsBuffer* getBackbuffer(GraphicsBuffer& pGraphicsBuffer);
	Vector2D getSize(GraphicsBuffer& graphicsBuffer) { return graphicsBuffer.getSize(); }
	void flip();

	void drawBackbuffer(const Vector2D& targetLoc, Sprite& sprite, float scale); 
	void drawGraphicsBuffer(GraphicsBuffer& targetGraphicsBuffer, const Vector2D& targetLoc, Sprite& sprite, float scale);

	void drawBackbuffer(const Vector2D& targetLoc, GraphicsBuffer& destination, float scale); 
	void drawGraphicsBuffer(GraphicsBuffer& targetGraphicsBuffer, const Vector2D& targetLoc, GraphicsBuffer& destination, float scale); 

	void writeTextToBackbuffer(const Vector2D& targetLoc, Font& font, Color& color, const std::string& text, bool alignCenter);
	void writeTextToGraphicsBuffer(GraphicsBuffer& targetGraphicsBuffer, const Vector2D& targetLoc, Font& font,  Color& color, const std::string& text, bool alignCenter);
	void saveBuffer(GraphicsBuffer& buffer, const std::string& filename);

	void setBitmapToColor(GraphicsBuffer& bitmap, Color& color);

	void drawButtons(int amount, float spacing, float size, float x1, float y1, Color& color);
private:
	ALLEGRO_DISPLAY* mDisplay;
};