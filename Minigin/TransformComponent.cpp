#include "TransformComponent.h"

void dae::TransformComponent::Update()
{
}

void dae::TransformComponent::SetPosition(const float x, const float y, const float z)
{
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;

	this->GetOwner().lock()->SetLocalPosition(this->GetOwner().lock()->GetWorldPosition() - m_Position);
}