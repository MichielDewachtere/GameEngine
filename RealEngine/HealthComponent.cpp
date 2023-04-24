#include "stdafx.h"
#include "HealthComponent.h"

#include <iostream>

#include "TransformComponent.h"

real::HealthComponent::HealthComponent(GameObject* pOwner, int lives)
	: Component(pOwner)
	, m_Lives(lives)
{
}

void real::HealthComponent::Update()
{
}

void real::HealthComponent::Damage()
{
	--m_Lives;

	healthChanged.Notify();

	if (m_Lives == 0)
		Kill();
	else
		Respawn();
}

void real::HealthComponent::Kill()
{
	//GetOwner()->Destroy();
	std::cout << "HealthComponent > GamObject should get destroyed";
}

void real::HealthComponent::Respawn()
{
	const auto transformComponent = this->GetOwner()->GetComponent<real::TransformComponent>();

	if (transformComponent == nullptr)
		return;

	transformComponent->SetWorldPosition(m_SpawnPoint);
}