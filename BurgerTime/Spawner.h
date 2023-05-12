#ifndef SPAWNER_H
#define SPAWNER_H

#include <Component.h>

class Spawner final : public real::Component
{
public:
	enum class EnemyTypes
	{
		hotDog,
		egg,
		pickle
	};

	explicit Spawner(real::GameObject* pGameObject, const EnemyTypes type = EnemyTypes::hotDog) : Component(pGameObject), m_Type(type) {}
	virtual ~Spawner() override = default;
	Spawner(const Spawner& other) = delete;
	Spawner operator=(Spawner& rhs) = delete;
	Spawner(Spawner&& other) = delete;
	Spawner operator=(Spawner&& rhs) = delete;

	void Update() override;
	bool CanRender() const override { return false; }

	void SpawnEnemy();

	void SetType(const EnemyTypes type) { m_Type = type; }
private:
	EnemyTypes m_Type{};
	float m_AccuTime{};

	bool m_NeedNewEnemy{};
};

#endif // SPAWNER_H