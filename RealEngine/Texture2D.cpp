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

void real::Texture2D::Scale(const float uniformScale) const
{
	Scale(uniformScale, uniformScale);
}

void real::Texture2D::Scale(const float scaleX, const float scaleY) const
{
	const auto pRenderer = SDLRenderer::GetInstance().GetSDLRenderer();

	// Set the scale factors
	SDL_RenderSetScale(pRenderer, scaleX, scaleY);

	// Render the texture at the scaled size
	SDL_RenderCopy(pRenderer, m_pTexture, nullptr, nullptr);

	// Reset the scale factors to their original values (1.0)
	SDL_RenderSetScale(pRenderer, 1.0, 1.0);
}

SDL_Texture* real::Texture2D::GetSDLTexture() const
{
	return m_pTexture;
}

real::Texture2D::Texture2D(SDL_Texture* texture)
{
	m_pTexture = texture;
}
