#ifndef BASEENEMY_H
#define BASEENEMY_H

#include <Component.h>
#include <ColliderComponent.h>

class BaseEnemy : public real::Component
{
public:
	explicit BaseEnemy(real::GameObject* pOwner) : Component(pOwner) {}
	virtual ~BaseEnemy() override = default;
	BaseEnemy(const BaseEnemy& other) = delete;
	BaseEnemy operator=(const BaseEnemy& rhs) = delete;
	BaseEnemy(BaseEnemy&& other) = delete;
	BaseEnemy operator=(BaseEnemy&& rhs) = delete;

	void Start() override;

	void Update() override;
	bool CanRender() const override { return false; }

private:
	enum class EnemyState
	{
		outOfBounds,
		moveX,
		moveY,
		fall,
		crushed,
		stun,
		dead
	};
	EnemyState m_CurrentState{ EnemyState::outOfBounds };

	std::vector<std::unique_ptr<real::GameObject>> m_PlayerPtrs;

	float m_Speed{ 40 };
	bool m_CanTurn = true;

	float m_DeathTimer{};
	float m_MaxDeathTime{ 3.f };

	std::vector<std::unique_ptr<real::GameObject>> m_StairPtrs;
	std::vector<std::unique_ptr<real::GameObject>> m_HiddenStairPtrs;
	real::gameobject_id m_CurrentStair = 0;

	std::vector<std::unique_ptr<real::GameObject>> m_FloorPtrs;
	real::gameobject_id m_CurrentPlatform = 0;

	std::vector<std::unique_ptr<real::GameObject>> m_IngredientPtrs;
	bool m_IsOnIngredient = false;
	real::GameObject* m_pCurrentIngredient{};

	glm::vec2 m_Direction{};

	real::GameObject* GetClosestPlayer(const std::vector<real::GameObject*>& playerPtrs);
	bool PlayerHit(real::GameObject* pPlayer) const;

	bool CheckForStairs(real::TransformComponent* playerTransform);
	bool CheckForHiddenStairs();
	bool CheckForPlatforms(real::TransformComponent* playerTransform);
	void CheckForIngredients();

	void MoveEnemy();
	void Fall();

	real::ColliderComponent* m_pWorldBorder{};
	bool IsInBounds() const;

	enum class Direction
	{
		up,
		down,
		left,
		right
	};
	bool CanMoveTo(const glm::vec2& enemyPos, const glm::vec2& enemyColliderSize, const real::ColliderComponent& structureCollider, Direction direction, bool
	               entireCollider = true) const;
};

#endif // BASEENEMY_H