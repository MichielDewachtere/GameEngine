#ifndef INGREDIENTPART_H
#define INGREDIENTPART_H

#include <Component.h>

namespace real
{
	class ColliderComponent;
}

class IngredientPart : public real::Component
{
public:
	explicit IngredientPart(real::GameObject* pOwner) : Component(pOwner) {}
	virtual ~IngredientPart() override = default;
	IngredientPart(const IngredientPart& other) = delete;
	IngredientPart& operator=(const IngredientPart& other) = delete;
	IngredientPart(IngredientPart&& other) = delete;
	IngredientPart& operator=(IngredientPart&& other) = delete;

	void Update() override;
	bool CanRender() const override { return false; }

	bool IsTriggered() const { return m_IsTriggered; }
	void SetIsTriggered(bool isTriggered) { m_IsTriggered = isTriggered; }

	void IsPlayerOnBurger(bool onBurger, real::ColliderComponent* player) { m_PlayerIsOnBurger = onBurger; m_Player = player; }

	void Reset();
private:
	bool m_IsTriggered{};
	bool m_PlayerIsOnBurger{};
	real::ColliderComponent* m_Player{};

};

#endif // INGREDIENTPART_H