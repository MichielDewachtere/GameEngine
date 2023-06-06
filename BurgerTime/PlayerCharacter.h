#ifndef PLAYERCHARACTER_H
#define PLAYERCHARACTER_H

#include <Component.h>
#include <glm/vec2.hpp>

#include "Subject.h"

class PlayerCharacter : public real::Component
{
public:
	explicit PlayerCharacter(real::GameObject* pOwner);
	virtual ~PlayerCharacter() override = default;
	PlayerCharacter(const PlayerCharacter& other) = delete;
	PlayerCharacter& operator=(const PlayerCharacter& rhs) = delete;
	PlayerCharacter(PlayerCharacter&& other) = delete;
	PlayerCharacter& operator=(PlayerCharacter&& rhs) = delete;

	void Update() override;
	bool CanRender() const override { return false; }

	void ThrowPepper();
	real::Subject<bool> pepperThrown{};

	void SetDirection(glm::vec2 direction) { m_CurrentDirection = direction; }

private:
	bool m_PepperThrown{};
	float m_AccuTime{};
	float m_MaxStunTime{ 1.f };
	glm::vec2 m_CurrentDirection{};
};

#endif // PLAYERCHARACTER_H