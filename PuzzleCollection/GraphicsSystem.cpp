#include "GraphicsSystem.h"
GraphicsSystem::GraphicsSystem()
{
	mDisplay = NULL;
}

GraphicsSystem::~GraphicsSystem() 
{
	al_destroy_display(mDisplay);
	cleanup();
}

void GraphicsSystem::createDisplay(int width, int height) 
{
	mDisplay = al_create_display(width, height);
	assert(mDisplay);
}

bool GraphicsSystem::initGraphics() 
{
	bool didInit1;
	bool didInit2;
	didInit1 = al_init();
	if (!didInit1) 
	{
		cout << "ERROR - System not initted\n";
		return false;
	}
	didInit1 = al_init_image_addon();
	didInit2 = al_init_primitives_addon();
	if (!didInit1 && !didInit2) 
	{
		cout << "ERROR - Graphics subsystem not initted\n";
		return false;
	}
	didInit1 = al_init_font_addon();
	didInit2 = al_init_ttf_addon();
	if (!didInit1 && !didInit2) 
	{
		cout << "ERROR - Font subsystem not initted\n";
		return false;
	}
	return true;
}

void GraphicsSystem::cleanup() 
{
	al_uninstall_system();
}

GraphicsBuffer* GraphicsSystem::getBackbuffer(GraphicsBuffer& pGraphicsBuffer) 
{
	pGraphicsBuffer.mBitmap = al_get_backbuffer(mDisplay);
	return &pGraphicsBuffer;
}

Vector2D GraphicsSystem::getSize(GraphicsBuffer& graphicsBuffer) 
{
	return graphicsBuffer.getSize();
}

void GraphicsSystem::flip() 
{
	al_flip_display();
}

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

void GraphicsSystem::writeTextToBackbuffer(const Vector2D& targetLoc, Font& font, Color& color, const std::string& text) 
{
	const ALLEGRO_COLOR pColor = al_map_rgba(color.getR(), color.getG(), color.getB(), color.getA());
	al_draw_text(font.mFont, pColor, targetLoc.getX(), targetLoc.getY(), ALLEGRO_ALIGN_LEFT, text.c_str());
}

void GraphicsSystem::writeTextToGraphicsBuffer(GraphicsBuffer& targetGraphicsBuffer, const Vector2D& targetLoc, Font& font, Color& color, const std::string& text) 
{
	ALLEGRO_BITMAP* pOldTarget = al_get_target_bitmap();
	al_set_target_bitmap(targetGraphicsBuffer.mBitmap);

	const ALLEGRO_COLOR pColor = al_map_rgba(color.getR(), color.getG(), color.getB(), color.getA());
	al_draw_text(font.mFont, pColor, targetLoc.getX(), targetLoc.getY(), ALLEGRO_ALIGN_LEFT, text.c_str());

	al_set_target_bitmap(pOldTarget);
}

void GraphicsSystem::saveBuffer(GraphicsBuffer& buffer, const std::string& filename) 
{
	al_save_bitmap(filename.c_str(), buffer.mBitmap);
}

void GraphicsSystem::setBitmapToColor(GraphicsBuffer& bitmap, Color& color)
{
	ALLEGRO_BITMAP* pOldTarget = al_get_target_bitmap();
	al_set_target_bitmap(bitmap.mBitmap);

	al_clear_to_color(al_map_rgba(color.getR(), color.getG(), color.getB(), color.getA()));

	al_set_target_bitmap(pOldTarget);
}