#include <stdexcept>
#include "TextComponent.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "TextureComponent.h"

void real::TextComponent::Update()
{
	if (m_NeedsUpdate)
	{
		const auto pTextureRenderer = this->GetOwner()->GetComponent<real::TextureComponent>();

		if (!pTextureRenderer)
			return;

		const auto surf = TTF_RenderText_Blended(m_pFont->GetFont(), m_pText.c_str(), m_Color);
		if (surf == nullptr) 
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr) 
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_FreeSurface(surf);

		pTextureRenderer->SetTexture(std::make_shared<Texture2D>(texture));

		m_NeedsUpdate = false;
	}
}

// This implementation uses the "dirty flag" pattern
void real::TextComponent::SetText(const std::string& text)
{
	m_pText = text;
	m_NeedsUpdate = true;
}

void real::TextComponent::SetFont(const std::shared_ptr<Font>& pFont)
{
	m_pFont = pFont;
	m_NeedsUpdate = true;
}

void real::TextComponent::SetColor(const SDL_Color& color)
{
	m_Color = color;
}

void real::TextComponent::SetColor(const Uint8 r, const Uint8 g, const Uint8 b, const Uint8 a)
{
	m_Color.r = r;
	m_Color.g = g;
	m_Color.b = b;
	m_Color.a = a;
}
