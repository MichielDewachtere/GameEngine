#ifndef MOVECOMMAND_H
#define MOVECOMMAND_H

#include "Command.h"

class MoveCommand final : public real::Command
{
public:
	explicit MoveCommand(real::GameObject* object, const glm::vec2& direction, const float speed = 50);
	virtual ~MoveCommand() override = default;
	MoveCommand(const MoveCommand& other) = delete;
	MoveCommand operator=(const MoveCommand& rhs) = delete;
	MoveCommand(MoveCommand&& other) = delete;
	MoveCommand operator=(MoveCommand&& rhs) = delete;

	void Execute() override;
	void SetDirection(const glm::vec2& direction) { m_Direction = direction; }
	void SetSpeed(const float speed) { m_Speed = speed; }

private:
	glm::vec2 m_Direction;
	float m_Speed{};
};

#endif // MOVECOMMAND_H