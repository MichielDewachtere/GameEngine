#include "stdafx.h"
#include "MoveCommand.h"

#include "TransformComponent.h"
#include "GameTime.h"

real::MoveCommand::MoveCommand(real::GameObject* object, const float speed)
    : Command(object)
    , m_Speed(speed)
{
}

void real::MoveCommand::Execute()
{
    // Get the TransformComponent of the game object associated with this MoveCommand.
    const auto pTransform = GetOwner()->GetComponent<real::TransformComponent>();

    // Check if the TransformComponent is valid.
    if (pTransform == nullptr)
    {
        throw std::runtime_error("Error: Could not execute MoveCommand - no TransformComponent found on associated GameObject. Please ensure the GameObject has a TransformComponent attached.");
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

    // Translate the TransformComponent to the new position.
    //pTransform->Translate(newPos);
    pTransform->SetWorldPosition(newPos);
}
