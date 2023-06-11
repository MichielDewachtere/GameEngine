#ifndef MOVECOMMAND_H
#define MOVECOMMAND_H

#include <vector>

#include <glm/vec2.hpp>

#include <Command.h>
#include <Observer.h>

class MoveCommand final : public real::Command,
	real::Observer<bool>
{
public:
	explicit MoveCommand(real::GameObject* object, const glm::vec2& direction, const float speed = 50);
	virtual ~MoveCommand() override;
	MoveCommand(const MoveCommand& other) = delete;
	MoveCommand operator=(const MoveCommand& rhs) = delete;
	MoveCommand(MoveCommand&& other) = delete;
	MoveCommand operator=(MoveCommand&& rhs) = delete;

	virtual void Start() override;

	void Execute() override;
	void SetDirection(const glm::vec2& direction) { m_Direction = direction; }
	void SetSpeed(const float speed) { m_Speed = speed; }
	void HandleEvent(bool) override;
	void OnSubjectDestroy() override {}

private:
	glm::vec2 m_Direction{};
	float m_Speed{};

	std::vector<real::GameObject*> m_StairPtrs{};
	std::vector<real::GameObject*> m_FloorPtrs{};
	real::GameObject* m_Boundary{};

	bool m_CanMove{};
	bool m_PlayerObserversAdded{};
};

#endif // MOVECOMMAND_H