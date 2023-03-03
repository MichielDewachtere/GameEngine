#include "TextureComponent.h"

#include "Renderer.h"
#include "TransformComponent.h"

void dae::TextureComponent::Render() const
{
	if (!m_pTexture)
		return;

	const auto pTransform = this->GetOwner().lock()->GetComponent<TransformComponent>();
	if (!pTransform)
		return;

	const glm::vec2& pos = pTransform->GetPosition();
	Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y);
}