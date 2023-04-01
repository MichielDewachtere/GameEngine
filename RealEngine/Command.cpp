#include "Command.h"

#include <iostream>

#include "GameObject.h"
#include "TransformComponent.h"
#include <memory>

#include "Input.h"
#include "Time.h"

#define MAX_THUMBSTICK_VALUE 32767

real::MoveCommand::MoveCommand(GameObject* object, const float speed)
	: Command(object)
	, m_Speed(speed)
{
}
void real::MoveCommand::Execute()
{
    // Get the TransformComponent of the game object associated with this MoveCommand.
    const auto pTransform = GetObject()->GetComponent<TransformComponent>();

    // Check if the TransformComponent is valid.
    if (pTransform == nullptr)
    {
        throw std::runtime_error("no transform component found when MoveCommand::Execute executed");
    }

    // Get the current position of the game object.
    const auto pos = pTransform->GetWorldPosition();

    // Flip the y direction so that the positive y direction is up.
    m_Direction.y *= -1;

    // Calculate the new position of the game object based on its current position,
    // its direction, and the elapsed time since the last frame.
    glm::vec2 newPos;
    newPos.x = pos.x + m_Direction.x * m_Speed * Time::GetInstance().GetElapsed();
    newPos.y = pos.y + m_Direction.y * m_Speed * Time::GetInstance().GetElapsed();

    // Set the new position of the game object.
    pTransform->Translate(newPos);
}
void real::TestCommand::Execute()
{
	std::cout << "test approved idk\n";
}
