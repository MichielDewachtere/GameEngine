#include "TextureComponent.h"

#include "Renderer.h"
#include "TransformComponent.h"

void dae::TextureComponent::Render() const
{
	if (!m_pTexture)
		return;

	const auto pTransform = this->GetOwner()->GetComponent<TransformComponent>();
	if (!pTransform)
		return;

	const glm::vec2& pos = pTransform->GetWorldPosition();
	Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y);
}