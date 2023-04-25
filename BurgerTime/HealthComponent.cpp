#include "stdafx.h"
#include "HealthComponent.h"

#include <iostream>

#include "LivesDisplay.h"
#include "TransformComponent.h"

HealthComponent::HealthComponent(real::GameObject* pOwner, int lives)
	: Component(pOwner)
	, m_Lives(lives)
{
}

void HealthComponent::Update()
{
}

void HealthComponent::Damage()
{
	--m_Lives;

	healthChanged.Notify();

	if (m_Lives == 0)
		Kill();
	else
		Respawn();
}

void HealthComponent::Kill()
{
	//GetOwner()->Destroy();
	std::cout << "HealthComponent > GamObject should get destroyed";
}

void HealthComponent::Respawn()
{
	const auto transformComponent = this->GetOwner()->GetComponent<real::TransformComponent>();

	if (transformComponent == nullptr)
		return;

	transformComponent->SetWorldPosition(m_SpawnPoint);
}