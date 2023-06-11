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

	explicit Spawner(real::GameObject* pGameObject) : Component(pGameObject) {}
	virtual ~Spawner() override = default;
	Spawner(const Spawner& other) = delete;
	Spawner operator=(Spawner& rhs) = delete;
	Spawner(Spawner&& other) = delete;
	Spawner operator=(Spawner&& rhs) = delete;

	void Update() override;
	bool CanRender() const override { return false; }

	void SpawnEnemy(const std::string& type);
	void ReSpawnEnemy(real::GameObject* pEnemy) const;

private:
	std::vector<EnemyTypes> m_Types;
	float m_AccuTime{};

	int m_NeedEnemies{};

	void SpawnEnemyType(const std::string& type, int points);
};

#endif // SPAWNER_H