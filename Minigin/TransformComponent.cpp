#include "TransformComponent.h"

void real::TransformComponent::Update()
{
}

const glm::vec3& real::TransformComponent::GetLocalPosition() const
{
	return m_LocalPosition;
}

const glm::vec3& real::TransformComponent::GetWorldPosition()
{
	if (m_PositionIsDirty)
		UpdateWorldPosition();
	return m_WorldPosition;
}

void real::TransformComponent::SetWorldPosition(const float x, const float y, const float z)
{
	m_WorldPosition.x = x;
	m_WorldPosition.y = y;
	m_WorldPosition.z = z;
}

void real::TransformComponent::SetWorldPosition(const glm::vec3& pos)
{
	m_WorldPosition = pos;
}

void real::TransformComponent::SetLocalPosition(float x, float y, float z)
{
	m_LocalPosition.x = x;
	m_LocalPosition.y = y;
	m_LocalPosition.z = z;

	SetPositionDirty();
}

void real::TransformComponent::SetLocalPosition(const glm::vec3& pos)
{
	m_LocalPosition = pos;

	SetPositionDirty();
}

void real::TransformComponent::UpdateWorldPosition()
{
	if (m_PositionIsDirty)
	{
		if (GetOwner()->GetParent() == nullptr)
			m_WorldPosition = m_LocalPosition;
		else
			m_WorldPosition = GetOwner()->GetParent()->GetComponent<TransformComponent>()->GetWorldPosition() + m_LocalPosition;
	}
	m_PositionIsDirty = false;
}

void real::TransformComponent::SetPositionDirty()
{
	m_PositionIsDirty = true;

	for (const auto& pChild : GetOwner()->GetChildren())
		pChild->GetComponent<TransformComponent>()->SetPositionDirty();
}
