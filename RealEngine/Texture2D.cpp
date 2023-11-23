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
	return { static_cast<float>(dst.w) * m_Scale.x,static_cast<float>(dst.h) * m_Scale.y };
}

void real::Texture2D::SetScale(const float uniformScale)
{
	SetScale(uniformScale, uniformScale);
}

void real::Texture2D::SetScale(const float scaleX, const float scaleY)
{
	m_Scale.x = scaleX;
	m_Scale.y = scaleY;
}

SDL_Texture* real::Texture2D::GetSDLTexture() const
{
	return m_pTexture;
}

real::Texture2D::Texture2D(SDL_Texture* texture)
{
	m_pTexture = texture;
}
