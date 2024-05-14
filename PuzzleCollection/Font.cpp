#include "Font.h"

Font::Font(const std::string& filename, int size) 
{
	mFont = al_load_ttf_font(filename.c_str(), size, 0);
	mFontSize = size;
}