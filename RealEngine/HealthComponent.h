#ifndef HEALTHCOMPONENT_H
#define HEALTHCOMPONENT_H

#include "stdafx.h"

#include "Component.h"

namespace real
{
	class HealthComponent final : public Component
	{
	public:
		explicit HealthComponent(GameObject* pOwner) : Component(pOwner) {}
		virtual ~HealthComponent() override = default;
		HealthComponent(const HealthComponent& other) = delete;
		HealthComponent& operator=(const HealthComponent& other) = delete;
		HealthComponent(HealthComponent&& other) = delete;
		HealthComponent& operator=(HealthComponent&& other) = delete;

		void Update() override;

		bool CanRender() const override { return false; }

		void SetHealth(const int health);

		void SetLives(const int lives) { m_Lives = lives; }
		int GetLives() const { return m_Lives; }

		void SetSpawnPoint(const glm::vec2& spawnPoint) { m_SpawnPoint = spawnPoint; }

		void Damage(const int amount = 0);
	private:
		int m_Health{};
		int m_CurrentHealth{};
		int m_Lives{};
		glm::vec2 m_SpawnPoint{};

		void Kill();
		void Respawn();
	};
}

#endif // HEALTHCOMPONENT_H