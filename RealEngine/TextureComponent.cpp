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
	SDLRenderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y);
}