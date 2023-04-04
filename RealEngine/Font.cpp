#include "stdafx.h"
#include "Font.h"

TTF_Font* real::Font::GetFont() const {
	return m_font;
}

real::Font::Font(const std::string& fullPath, unsigned int size) : m_font(nullptr), m_size(size)
{
	m_font = TTF_OpenFont(fullPath.c_str(), size);
	if (m_font == nullptr) 
	{
		throw std::runtime_error(std::string("Failed to load font: ") + SDL_GetError());
	}
}

real::Font::~Font()
{
	TTF_CloseFont(m_font);
}
