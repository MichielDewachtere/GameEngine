//#include "stdafx.h"
#include "TextComponent.h"

#include <SDL_ttf.h>

#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "TextureComponent.h"
#include "TransformComponent.h"

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

		const auto pTexture = std::make_shared<Texture2D>(texture);

		pTextureRenderer->SetTexture(pTexture);

		if (m_CenterText && m_IsCentered == false)
		{
			m_IsCentered = true;
			GetOwner()->GetComponent<TransformComponent>()->Translate(-static_cast<float>(pTexture->GetSize().x) / 2.f, 0);
		}
		else if (m_CenterText == false && m_IsCentered)
		{
			m_IsCentered = false;
			GetOwner()->GetComponent<TransformComponent>()->Translate(static_cast<float>(pTexture->GetSize().x) / 2.f, 0);
		}

		m_NeedsUpdate = false;
	}
}

// This implementation uses the "dirty flag" pattern
void real::TextComponent::SetText(const std::string& text)
{
	m_pText = text;
	m_IsCentered = false;
	m_NeedsUpdate = true;
}

void real::TextComponent::SetFont(const std::shared_ptr<Font>& pFont)
{
	m_pFont = pFont;
	m_IsCentered = false;
	m_NeedsUpdate = true;
}

void real::TextComponent::SetColor(const SDL_Color& color)
{
	m_Color = color;
}

void real::TextComponent::SetColor(const glm::vec4& color)
{
	m_Color.r = static_cast<Uint8>(color.r);
	m_Color.g = static_cast<Uint8>(color.g);
	m_Color.b = static_cast<Uint8>(color.b);
	m_Color.a = static_cast<Uint8>(color.a);
}

void real::TextComponent::SetColor(const Uint8 r, const Uint8 g, const Uint8 b, const Uint8 a)
{
	m_Color.r = r;
	m_Color.g = g;
	m_Color.b = b;
	m_Color.a = a;
}


void real::TextComponent::CenterText(bool centerText)
{
	m_CenterText = centerText;
	m_NeedsUpdate = true;
}
