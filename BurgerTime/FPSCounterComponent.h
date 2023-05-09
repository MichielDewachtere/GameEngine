#ifndef FPSCOUNTERCOMPONENT_H
#define FPSCOUNTERCOMPONENT_H

#include <Component.h>

class FPSCounterComponent final : public real::Component
{
public:
	explicit FPSCounterComponent(real::GameObject* pOwner) : Component(pOwner) {}
	virtual ~FPSCounterComponent() override = default;
	FPSCounterComponent(const FPSCounterComponent& other) = delete;
	FPSCounterComponent& operator=(const FPSCounterComponent& other) = delete;
	FPSCounterComponent(FPSCounterComponent&& other) = delete;
	FPSCounterComponent& operator=(FPSCounterComponent&& other) = delete;

	void Update() override;

	bool CanRender() const override { return false; }
private:
	const float m_UpdateInterval{ 0.1f };
	float m_Time{ m_UpdateInterval };
};

#endif // FPSCOUNTERCOMPONENT_H
