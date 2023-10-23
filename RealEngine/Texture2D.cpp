//#include "stdafx.h"
#include "Texture2D.h"

#include <SDL_rect.h>
#include <SDL_render.h>

#include "SDLRenderer.h"

real::Texture2D::~Texture2D()
{
	SDL_DestroyTexture(m_pTexture);
}

glm::ivec2 real::Texture2D::GetSize() const
{
	SDL_Rect dst;
	SDL_QueryTexture(GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	return { dst.w,dst.h };
}

SDL_Texture* real::Texture2D::GetSDLTexture() const
{
	return m_pTexture;
}

real::Texture2D::Texture2D(SDL_Texture* texture)
{
	m_pTexture = texture;
}
