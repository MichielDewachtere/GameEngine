#ifndef INGREDIENT_H
#define INGREDIENT_H

#include <Component.h>

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

	void PartIsTriggered();

private:
	void Fall();

	int m_AmountOfPartsTriggered{};
	std::vector<real::ColliderComponent*> m_PartPtrs;

	void NotifyParts(bool playerOnBurger, real::ColliderComponent* pFeet) const;
};

#endif // INGREDIENT_H
