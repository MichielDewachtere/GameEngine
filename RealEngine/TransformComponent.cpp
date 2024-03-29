//#include "stdafx.h"
#include "TransformComponent.h"

void real::TransformComponent::Update()
{
	if (/*changedWorldPosition.GetObservers().empty() == false && */m_PositionIsDirty)
		UpdateWorldPosition();
}

const glm::vec2& real::TransformComponent::GetLocalPosition() const
{
	return m_LocalPosition;
}

const glm::vec2& real::TransformComponent::GetWorldPosition()
{
	if (m_PositionIsDirty)
		UpdateWorldPosition();
	return m_WorldPosition;
}

void real::TransformComponent::SetWorldPosition(const float x, const float y)
{
	SetWorldPosition({ x,y });
}

void real::TransformComponent::SetWorldPosition(const glm::vec2& pos)
{
	m_WorldPosition = pos;

	changedWorldPosition.Notify(m_WorldPosition);

	for (const auto& pChild : GetOwner()->GetChildren())
		pChild->GetComponent<TransformComponent>()->SetPositionDirty();
}

void real::TransformComponent::SetLocalPosition(float x, float y)
{
	m_LocalPosition.x = x;
	m_LocalPosition.y = y;

	SetPositionDirty();
}

void real::TransformComponent::SetLocalPosition(const glm::vec2& pos)
{
	m_LocalPosition = pos;

	SetPositionDirty();
}

void real::TransformComponent::Translate(float x, float y)
{
	SetLocalPosition(GetLocalPosition() + glm::vec2{ x, y });
}
void real::TransformComponent::Translate(const glm::vec2& translation)
{
	SetLocalPosition(GetLocalPosition() + translation);
}

void real::TransformComponent::UpdateWorldPosition()
{
	if (m_PositionIsDirty == false)
		return;

	if (GetOwner()->GetParent() == nullptr || m_IgnoreParent)
		m_WorldPosition = m_LocalPosition;
	else
		m_WorldPosition = GetOwner()->GetParent()->GetComponent<TransformComponent>()->GetWorldPosition() + m_LocalPosition;

	changedWorldPosition.Notify(m_WorldPosition);

	m_PositionIsDirty = false;
}

void real::TransformComponent::SetPositionDirty()
{
	m_PositionIsDirty = true;

	for (const auto& pChild : GetOwner()->GetChildren())
		pChild->GetComponent<TransformComponent>()->SetPositionDirty();
}
