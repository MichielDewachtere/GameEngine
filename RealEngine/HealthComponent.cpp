#include "HealthComponent.h"

#include <iostream>

#include "TransformComponent.h"

void real::HealthComponent::Update()
{
}

void real::HealthComponent::SetHealth(const int health)
{
	m_Health = health;
	m_CurrentHealth = health;
}

void real::HealthComponent::Damage(const int amount)
{
	m_Health -= amount;

	if (m_Health <= 0 && m_Lives > 0)
	{
		--m_Lives;

		GetOwner()->NotifyObservers(Observer::GameEvent::actorDied);

		if (m_Lives == 0)
			Kill();
		else
			Respawn();
	}
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

	m_Health = m_CurrentHealth;
	transformComponent->SetWorldPosition(m_SpawnPoint);
}