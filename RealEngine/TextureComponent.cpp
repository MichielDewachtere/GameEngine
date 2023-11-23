//#include "stdafx.h"
#include "TextureComponent.h"

#include "SDLRenderer.h"
#include "TransformComponent.h"

void real::TextureComponent::Render() const
{
	const auto pTransform = this->GetOwner()->GetComponent<TransformComponent>();

	if (!pTransform)
		return;

	const glm::vec2& pos = pTransform->GetWorldPosition();

	for (const auto& [texture, offset] 
		: { std::make_pair(m_pTexture, m_RenderOffset), std::make_pair(m_pText, m_TextRenderOffset) })
	{
		if (texture == nullptr)
			continue;

		const glm::vec2& size = { static_cast<float>(texture->GetSize().x), static_cast<float>(texture->GetSize().y) };
		SDLRenderer::GetInstance().RenderTexture(*texture, pos.x - static_cast<float>(offset.x), pos.y - static_cast<float>(offset.y), size.x, size.y);

	}
}