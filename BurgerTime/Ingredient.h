#ifndef INGREDIENT_H
#define INGREDIENT_H

#include <Component.h>

#include "Subject.h"

namespace real
{
	class ColliderComponent;
}

class Ingredient final : public real::Component
{
public:
	explicit Ingredient(real::GameObject* pOwner) : Component(pOwner) {}
	virtual ~Ingredient() override = default;
	Ingredient(const Ingredient& other) = delete;
	Ingredient& operator=(const Ingredient& other) = delete;
	Ingredient(Ingredient&& other) = delete;
	Ingredient& operator=(Ingredient&& other) = delete;

	void Update() override;
	bool CanRender() const override { return false; }

	void IncreaseWeight() { ++m_PlatformsToSkip; }
	void DecreaseWeight() { --m_PlatformsToSkip; }

	void PartIsTriggered();

	void SetIsFalling(bool isFalling) { m_IsFalling = isFalling; }
	bool GetIsFalling() const { return m_IsFalling; }
	int GetFallSpeed() const { return m_FallSpeed; }

	real::Subject<Ingredient&> landedOnPlate{};
	real::Subject<int> burgerDropped{};
private:
	//bool m_IsActive{ true };

	int m_AmountOfPartsTriggered{};
	std::vector<real::ColliderComponent*> m_PartPtrs;

	bool m_IsFalling{};
	const int m_FallSpeed{ 96 };

	int m_PlatformsToSkip{ 0 };
	int m_PlatformsCrossed{ 1 };
	real::gameobject_id m_CurrentPlatform{ 0 };

	void InitCurrentPlatform();
	void ResetBurger();
	void Fall();
	void NotifyParts(bool playerOnBurger, real::ColliderComponent* pFeet) const;
};

#endif // INGREDIENT_H
