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
	std::vector<std::unique_ptr<real::GameObject>> m_PlayerPtrs;

	float m_Speed{ 40 };
	bool m_CanTurn = true;

	std::vector<std::unique_ptr<real::GameObject>> m_StairPtrs;
	bool m_IsOnStair = false;
	real::gameobject_id m_CurrentStair = 0;

	std::vector<std::unique_ptr<real::GameObject>> m_FloorPtrs;
	bool m_IsOnFloor = false;
	real::gameobject_id m_CurrentPlatform = 0;

	std::vector<std::unique_ptr<real::GameObject>> m_IngredientPtrs;
	bool m_IsOnIngredient = false;
	bool m_IsFalling = false;
	real::GameObject* m_pCurrentIngredient{};

	glm::vec2 m_Direction{};

	real::GameObject* GetClosestPlayer(const std::vector<real::GameObject*>& playerPtrs);
	bool PlayerHit(real::GameObject* pPlayer) const;

	bool CheckForStairs(real::TransformComponent* playerTransform);
	bool CheckForPlatforms(real::TransformComponent* playerTransform);
	void CheckForIngredients();

	void MoveEnemy();
	void Fall();

	enum class Direction
	{
		up,
		down,
		left,
		right
	};
	bool CanMoveTo(const glm::vec2& enemyPos, const glm::vec2& enemyColliderSize, const real::ColliderComponent& structureCollider, Direction direction) const;
};

#endif // BASEENEMY_H