#ifndef ROTATORCOMPONENT_H
#define ROTATORCOMPONENT_H

#include <glm/vec2.hpp>

#include <Component.h>

class RotatorComponent final : public real::Component
{
public:
	explicit RotatorComponent(real::GameObject* pOwner) : Component(pOwner) {}
	~RotatorComponent() override = default;
	RotatorComponent(const RotatorComponent& other) = delete;
	RotatorComponent operator=(const RotatorComponent& rhs) = delete;
	RotatorComponent(RotatorComponent&& other) = delete;
	RotatorComponent operator=(RotatorComponent&& rhs) = delete;

	void Update() override;

	void SetPivot(const glm::vec2& pivot) { m_Pivot = pivot; }
	void SetSpeed(const float speed) { m_Speed = speed; }

	bool CanRender() const override { return false; }

private:
	glm::vec2 m_Pivot{};
	float m_Speed{ 50 };
	float m_Angle{};
};

#endif // ROTATORCOMPONENT_H 