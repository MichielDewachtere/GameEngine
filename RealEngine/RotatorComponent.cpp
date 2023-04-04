#include "stdafx.h"
#include "RotatorComponent.h"

#include "Time.h"
#include "TransformComponent.h"

void real::RotatorComponent::Update()
{
	const auto pTransform = GetOwner()->GetComponent<TransformComponent>();
	
	if (!pTransform)
		return;
	
	const auto pParent = GetOwner()->GetParent();

	if (!pParent)
		m_Pivot = { 0,0 };
	else
		m_Pivot = pParent->GetComponent<TransformComponent>()->GetWorldPosition();

	const auto dt = Time::GetInstance().GetElapsed();
	const float radius = distance(pTransform->GetWorldPosition(), m_Pivot);
	glm::vec3 newPos;
	
	m_Angle += m_Speed * dt;
	
	if (m_Angle >= 360)
		m_Angle = 0;

	newPos.x = radius * glm::cos(glm::radians(m_Angle));
	newPos.y = radius * glm::sin(glm::radians(m_Angle));

	pTransform->SetLocalPosition(newPos);
}