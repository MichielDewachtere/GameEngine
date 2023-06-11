#ifndef HEALTHCOMPONENT_H
#define HEALTHCOMPONENT_H

#include <glm/vec2.hpp>

#include <Component.h>
#include <Subject.h>

#include "BaseEnemy.h"

class HealthComponent final : public real::Component,
	//public real::Observer<int, int>,
	public real::Observer<>,
	public real::Observer<real::Scene&>
{
public:
	explicit HealthComponent(real::GameObject* pOwner);
	virtual ~HealthComponent() override;
	HealthComponent(const HealthComponent& other) = delete;
	HealthComponent& operator=(const HealthComponent& other) = delete;
	HealthComponent(HealthComponent&& other) = delete;
	HealthComponent& operator=(HealthComponent&& other) = delete;

	void Start() override;
	void Update() override;

	bool CanRender() const override { return false; }

	void HandleEvent() override;
	void HandleEvent(real::Scene&) override;
	//void HandleEvent(int, int) override;
	void OnSubjectDestroy() override {}

	void SetLives(const int lives) { m_Lives = lives; }
	int GetLives() const { return m_Lives; }

	void SetSpawnPoint(const glm::vec2& spawnPoint) { m_SpawnPoint = spawnPoint; }

	void Damage();
	bool IsDead() const { return m_PlayerDied; }

	//real::Subject<> playerDied;
	//real::Subject<int, int> onStatChanged;
	real::Subject<bool> playerDied;

private:
	static inline int m_Lives{ 4 };
	glm::vec2 m_SpawnPoint{};

	bool m_PlayerDied{};
	float m_AccuTime{};
	float m_TimeToRespawn{ 4 };
	bool m_SpriteChanged{};

	std::vector<BaseEnemy*> m_EnemyPtrs;
	
	void Respawn();
};

#endif // HEALTHCOMPONENT_H