//#include "stdafx.h"
#include "TextureComponent.h"

#include "SDLRenderer.h"
#include "TransformComponent.h"

void real::TextureComponent::Render() const
{
	if (!m_pTexture)
		return;

	const auto pTransform = this->GetOwner()->GetComponent<TransformComponent>();
	if (!pTransform)
		return;

	const glm::vec2& pos = pTransform->GetWorldPosition();
	const glm::vec2& size = { static_cast<float>(m_pTexture->GetSize().x) * m_Scale.x, static_cast<float>(m_pTexture->GetSize().y) * m_Scale.y };
	SDLRenderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y, size.x, size.y);
}

void real::TextureComponent::Scale(const float uniformScale)
{
	Scale(uniformScale, uniformScale);
}

void real::TextureComponent::Scale(const float scaleX, const float scaleY)
{
	m_Scale.x = scaleX;
	m_Scale.y = scaleY;
}