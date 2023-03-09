#include "RotatorComponent.h"

#include "Time.h"
#include "TransformComponent.h"

void dae::RotatorComponent::Update()
{
	const auto pTransform = this->GetOwner().lock()->GetComponent<TransformComponent>();
	
	if (!pTransform)
		return;

	// TODO: rotates always around parent create bool or smthn
	m_Pivot = this->GetOwner().lock()->GetParent()->GetWorldPosition();

	const auto dt = Time::GetInstance().GetElapsed();
	const float radius = distance(this->GetOwner().lock()->GetWorldPosition(), m_Pivot);
	glm::vec3 newPos;

	//TODO: m_Angle keeps increasing
	m_Angle += m_Speed * dt;

	newPos.x = radius * glm::cos(glm::radians(m_Angle));
	newPos.y = radius * glm::sin(glm::radians(m_Angle));
	newPos.z = pTransform->GetPosition().z;

	pTransform->SetPosition(m_Pivot + newPos);
}