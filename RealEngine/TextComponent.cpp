//#include "stdafx.h"
#include "TextComponent.h"

#include <SDL_ttf.h>

#include "Font.h"
#include "SDLRenderer.h"
//#include "SDLFont.h"
#include "SDLResourceManager.h"
//#include "SDLTexture2D.h"
#include "Texture2D.h"
#include "TextureComponent.h"
#include "TransformComponent.h"

real::TextComponent::TextComponent(GameObject* pOwner)
	: Component(pOwner)
{
	m_OriginalPos = GetOwner()->GetComponent<TransformComponent>()->GetWorldPosition();
}

void real::TextComponent::Update()
{
	if (!m_NeedsUpdate)
		return;

	const auto pTextureRenderer = this->GetOwner()->GetComponent<real::TextureComponent>();

	if (!pTextureRenderer)
		return;

	//m_OriginalPos = GetOwner()->GetComponent<TransformComponent>()->GetWorldPosition();

	//const auto surf = TTF_RenderText_Blended(m_pFont->GetFont(), m_pText.c_str(), m_Color);
	//if (surf == nullptr)
	//{
	//	throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
	//}
	//auto texture = SDL_CreateTextureFromSurface(SDLRenderer::GetInstance().GetSDLRenderer(), surf);
	//if (texture == nullptr)
	//{
	//	throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
	//}
	//SDL_FreeSurface(surf);


	//// TODO: mayba add texture array in texturecomponent?
	//const auto pTexture = std::make_shared<SDLTexture2D>(texture);

	const auto pTexture = m_pFont->CreateTexture(m_pText, m_Color);

	glm::ivec2 renderOffset;

	HandleVerticalAlignment(pTexture->GetSize(), renderOffset);
	HandleHorizontalAlignment(pTexture->GetSize(), renderOffset);

	pTextureRenderer->SetText(pTexture);
	pTextureRenderer->SetTextRenderOffset(renderOffset);

	m_NeedsUpdate = false;
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

void real::TextComponent::SetColor(const glm::u8vec4& color)
{
	m_Color = color;
}

void real::TextComponent::SetColor(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a)
{
	m_Color.r = r;
	m_Color.g = g;
	m_Color.b = b;
	m_Color.a = a;
}

void real::TextComponent::ChangeHorizontalAlignment(HorizontalAlignment newAlignment)
{
	if (m_CurHorizontalAlignment != newAlignment)
	{
		m_CurHorizontalAlignment = newAlignment;
		m_NeedsUpdate = true;
	}
}

void real::TextComponent::ChangeVerticalAlignment(VerticalAlignment newAlignment)
{
	if (m_CurVerticalAlignment != newAlignment)
	{
		m_CurVerticalAlignment = newAlignment;
		m_NeedsUpdate = true;
	}
}

void real::TextComponent::HandleHorizontalAlignment(const glm::vec2& textureSize, glm::ivec2& renderOffset) const
{
	switch (m_CurHorizontalAlignment)
	{
	case HorizontalAlignment::left:
	{
		renderOffset.x = static_cast<int>(textureSize.x);
		break;
	}
	case HorizontalAlignment::center:
	{
		renderOffset.x = static_cast<int>(textureSize.x / 2.f);
		break;
	}
	case HorizontalAlignment::right:
	{
		renderOffset.x = 0;
		break;
	}
	}
}

void real::TextComponent::HandleVerticalAlignment(const glm::vec2& textureSize, glm::ivec2& renderOffset) const
{
	switch (m_CurVerticalAlignment)
	{
	case VerticalAlignment::up:
	{
		renderOffset.y = static_cast<int>(textureSize.y);
		break;
	}
	case VerticalAlignment::center:
	{
		renderOffset.y = static_cast<int>(textureSize.y / 2.f);
		break;
	}
	case VerticalAlignment::down:
	{
		renderOffset.y = 0;
		break;
	}
	}
}
