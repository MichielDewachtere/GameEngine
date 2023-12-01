//#include "stdafx.h"
#include "SDLTexture2D.h"

#include <SDL_rect.h>
#include <SDL_render.h>

#include "SDLRenderer.h"

real::SDLTexture2D::~SDLTexture2D()
{
	SDL_DestroyTexture(m_pTexture);
}

glm::ivec2 real::SDLTexture2D::GetSize() const
{
	SDL_Rect dst;
	SDL_QueryTexture(GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	return { static_cast<float>(dst.w) * m_Scale.x,static_cast<float>(dst.h) * m_Scale.y };
}

SDL_Texture* real::SDLTexture2D::GetSDLTexture() const
{
	return m_pTexture;
}

real::SDLTexture2D::SDLTexture2D(SDL_Texture* texture)
{
	m_pTexture = texture;
}
