//#include "stdafx.h"
#include "SDLFont.h"

#include <SDL_ttf.h>
#include <SDL_error.h>
#include <stdexcept>
#include <string>

#include "Locator.h"
#include "LoggingRenderer.h"
#include "SDLRenderer.h"
#include "SDLTexture2D.h"

TTF_Font* real::SDLFont::GetFont() const
{
	return m_pFont;
}

std::shared_ptr<real::Texture2D> real::SDLFont::CreateTexture(const std::string& text, glm::u8vec4 color)
{
	const SDL_Color sdlColor{ color.r, color.g, color.b, color.a };

	const auto surf = TTF_RenderText_Blended(m_pFont, text.c_str(), sdlColor);
	if (surf == nullptr)
	{
		throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
	}

	SDL_Renderer* renderer;
#ifdef _DEBUG
	renderer = dynamic_cast<SDLRenderer*>(&dynamic_cast<LoggingRenderer*>(&Locator::GetRenderSystem())->GetRenderer())->GetSDLRenderer();
#else
	renderer = dynamic_cast<SDLRenderer*>(&Locator::GetRenderSystem())->GetSDLRenderer();
#endif

	auto texture = SDL_CreateTextureFromSurface(renderer, surf);
	if (texture == nullptr)
	{
		throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
	}
	SDL_FreeSurface(surf);

	return std::make_shared<SDLTexture2D>(texture);
}

real::SDLFont::SDLFont(const std::string& fullPath, unsigned int size) : m_pFont(nullptr)/*, m_Size(size)*/
{
	m_pFont = TTF_OpenFont(fullPath.c_str(), static_cast<int>(size));
	if (m_pFont == nullptr) 
	{
		throw std::runtime_error(std::string("Failed to load font: ") + SDL_GetError());
	}
}

real::SDLFont::~SDLFont()
{
	TTF_CloseFont(m_pFont);
}
