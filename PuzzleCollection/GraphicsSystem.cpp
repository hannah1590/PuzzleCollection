#include "GraphicsSystem.h"

// Default contructor
GraphicsSystem::GraphicsSystem()
{
	mDisplay = NULL;
}

// Destroys allegro display
GraphicsSystem::~GraphicsSystem() 
{
	al_destroy_display(mDisplay);
	cleanup();
}

// Creates the screen
void GraphicsSystem::createDisplay(int width, int height) 
{
	mDisplay = al_create_display(width, height);
	assert(mDisplay);
}

// Inits all allegro systems needed
bool GraphicsSystem::initGraphics() 
{
	bool didInit1;
	bool didInit2;

	// Inits main system
	didInit1 = al_init();
	if (!didInit1) 
	{
		cout << "ERROR - System not initted\n";
		return false;
	}

	// Inits graphics subsystem
	didInit1 = al_init_image_addon();
	didInit2 = al_init_primitives_addon();
	if (!didInit1 && !didInit2) 
	{
		cout << "ERROR - Graphics subsystem not initted\n";
		return false;
	}

	// Inits font subsystem
	didInit1 = al_init_font_addon();
	didInit2 = al_init_ttf_addon();
	if (!didInit1 && !didInit2) 
	{
		cout << "ERROR - Font subsystem not initted\n";
		return false;
	}
	return true;
}

// Uninstalls allegro
void GraphicsSystem::cleanup() 
{
	al_uninstall_system();
}

// Gets the current backbuffer of the specificed graphics buffer
GraphicsBuffer* GraphicsSystem::getBackbuffer(GraphicsBuffer& pGraphicsBuffer) 
{
	pGraphicsBuffer.mBitmap = al_get_backbuffer(mDisplay);
	return &pGraphicsBuffer;
}

// Makes the current display visible
void GraphicsSystem::flip() 
{
	al_flip_display();
}

// Draw back buffer based on sprite location
void GraphicsSystem::drawBackbuffer(const Vector2D& targetLoc, Sprite& sprite, float scale) 
{
	float sx = sprite.getSoureLoc().getX();
	float sy = sprite.getSoureLoc().getY();
	float sw = sprite.getSize().getX();
	float sh = sprite.getSize().getY();
	float dx = targetLoc.getX();
	float dy = targetLoc.getY();
	float dw = sw * scale;
	float dh = sh * scale;

	al_draw_scaled_bitmap(sprite.getGraphicsBuffer()->mBitmap, sx, sy, sw, sh, dx, dy, dw, dh, 0);
}

// Draws sprite graphics buffer ontop of current buffer
void GraphicsSystem::drawGraphicsBuffer(GraphicsBuffer& targetGraphicsBuffer, const Vector2D& targetLoc, Sprite& sprite, float scale) 
{
	ALLEGRO_BITMAP* pOldTarget = al_get_target_bitmap();
	al_set_target_bitmap(targetGraphicsBuffer.mBitmap);

	float sx = sprite.getSoureLoc().getX();
	float sy = sprite.getSoureLoc().getY();
	float sw = sprite.getSize().getX();
	float sh = sprite.getSize().getY();
	float dx = targetLoc.getX();
	float dy = targetLoc.getY();
	float dw = sw * scale;
	float dh = sh * scale;

	al_draw_scaled_bitmap(sprite.getGraphicsBuffer()->mBitmap, sx, sy, sw, sh, dx, dy, dw, dh, 0);

	al_set_target_bitmap(pOldTarget);
} 

// Draw backbuffer at a specific location
void GraphicsSystem::drawBackbuffer(const Vector2D& targetLoc, GraphicsBuffer& destination, float scale) 
{
	float sx = 0.0f;
	float sy = 0.0f;
	float sw = destination.getSize().getX();
	float sh = destination.getSize().getY();
	float dx = targetLoc.getX();
	float dy = targetLoc.getY();
	float dw = sw * scale;
	float dh = sh * scale;

	al_draw_scaled_bitmap(destination.mBitmap, sx, sy, sw, sh, dx, dy, dw, dh, 0);
}

// Draw graphics buffer on top of current buffer at a specific location
void GraphicsSystem::drawGraphicsBuffer(GraphicsBuffer& targetGraphicsBuffer, const Vector2D& targetLoc, GraphicsBuffer& destination, float scale) 
{
	ALLEGRO_BITMAP* pOldTarget = al_get_target_bitmap();
	al_set_target_bitmap(targetGraphicsBuffer.mBitmap);

	float sx = 0.0f;
	float sy = 0.0f;
	float sw = destination.getSize().getX();
	float sh = destination.getSize().getY();
	float dx = targetLoc.getX();
	float dy = targetLoc.getY();
	float dw = sw * scale;
	float dh = sh * scale;

	al_draw_scaled_bitmap(destination.mBitmap, sx, sy, sw, sh, dx, dy, dw, dh, 0);

	al_set_target_bitmap(pOldTarget);
} 

// Write text on back buffer at location
void GraphicsSystem::writeTextToBackbuffer(const Vector2D& targetLoc, Font& font, Color& color, const std::string& text, bool alignCenter) 
{
	const ALLEGRO_COLOR pColor = al_map_rgba(color.getR(), color.getG(), color.getB(), color.getA());
	if(alignCenter)
		al_draw_text(font.mFont, pColor, targetLoc.getX(), targetLoc.getY(), ALLEGRO_ALIGN_CENTER, text.c_str());
	else
		al_draw_text(font.mFont, pColor, targetLoc.getX(), targetLoc.getY(), ALLEGRO_ALIGN_LEFT, text.c_str());
}

// Write text on top of current buffer at location
void GraphicsSystem::writeTextToGraphicsBuffer(GraphicsBuffer& targetGraphicsBuffer, const Vector2D& targetLoc, Font& font, Color& color, const std::string& text, bool alignCenter)
{
	ALLEGRO_BITMAP* pOldTarget = al_get_target_bitmap();
	al_set_target_bitmap(targetGraphicsBuffer.mBitmap);

	const ALLEGRO_COLOR pColor = al_map_rgba(color.getR(), color.getG(), color.getB(), color.getA());
	if(alignCenter)
		al_draw_text(font.mFont, pColor, targetLoc.getX(), targetLoc.getY(), ALLEGRO_ALIGN_CENTER, text.c_str());
	else
	al_draw_text(font.mFont, pColor, targetLoc.getX(), targetLoc.getY(), ALLEGRO_ALIGN_LEFT, text.c_str());

	al_set_target_bitmap(pOldTarget);
}

// Saves the current buffer to an image file
void GraphicsSystem::saveBuffer(GraphicsBuffer& buffer, const std::string& filename) 
{
	al_save_bitmap(filename.c_str(), buffer.mBitmap);
}

// Make bitmap a specific color
void GraphicsSystem::setBitmapToColor(GraphicsBuffer& bitmap, Color& color)
{
	ALLEGRO_BITMAP* pOldTarget = al_get_target_bitmap();
	al_set_target_bitmap(bitmap.mBitmap);

	al_clear_to_color(al_map_rgba(color.getR(), color.getG(), color.getB(), color.getA()));

	al_set_target_bitmap(pOldTarget);
}

// Draws squares on the screen to act as buttons
void GraphicsSystem::drawButtons(int amount, float spacing, float size, float x1, float y1, Color& color)
{
	const ALLEGRO_COLOR pColor = al_map_rgba(color.getR(), color.getG(), color.getB(), color.getA());

	for (int i = 0; i < amount; i++)
	{
		float x = x1;
		float y = y1 + (spacing * i);
		al_draw_filled_rectangle(x, y, x + size, y + size, pColor);
	}
}
