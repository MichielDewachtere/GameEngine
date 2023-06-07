//#include "stdafx.h"
#include "MoveCommand.h"

#include <ColliderComponent.h>
#include <TransformComponent.h>
#include <GameTime.h>
#include <Input.h>
#include <iostream>
#include <Scene.h>
#include <SceneManager.h>

#include "GameInfo.h"
#include "PlayerCharacter.h"

MoveCommand::MoveCommand(real::GameObject* object, const glm::vec2& direction, const float speed)
    : Command(object)
	, m_Direction(direction)
    , m_Speed(speed)
{
    // Flip the y direction so that the positive y direction is up.
    m_Direction.y *= -1;
}

void MoveCommand::Start()
{
    m_StairPtrs = real::SceneManager::GetInstance().GetActiveScene().FindObjectsWithTag(Tags::stair);
    m_FloorPtrs = real::SceneManager::GetInstance().GetActiveScene().FindObjectsWithTag(Tags::floor);
    const auto boundaryPtrs = real::SceneManager::GetInstance().GetActiveScene().FindObjectsWithTag(Tags::boundary);
    if (boundaryPtrs.empty() == false)
        m_Boundary = boundaryPtrs[0];
}

void MoveCommand::Execute()
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

    const auto& collider = GetOwner()->GetComponent<real::ColliderComponent>();

    real::ColliderComponent futureCollider{ nullptr, collider->GetSize() };

    // Calculate the new position of the game object based on its current position,
    // its direction, and the elapsed time since the last frame.
    glm::vec2 newPos = pos;
    glm::vec2 futurePos;
    constexpr float epsilon = 0.01f;
    futurePos.x = pos.x + m_Direction.x * m_Speed * real::Time::GetInstance().GetElapsed() + epsilon;
    futurePos.y = pos.y + m_Direction.y * m_Speed * real::Time::GetInstance().GetElapsed() + epsilon;
    futureCollider.SetPosition(futurePos);

    if (m_Boundary->HasComponent<real::ColliderComponent>())
    {
        if (m_Boundary->GetComponent<real::ColliderComponent>()->IsEntireColliderOverlapping(futureCollider) == false)
	            return;
    }

	// if owner overlaps with ground

    for (const auto& floor : m_FloorPtrs)
    {
        if (floor->HasComponent<real::ColliderComponent>() == false)
            continue;

        if (floor->GetComponent<real::ColliderComponent>()->IsEntireColliderOverlapping(futureCollider))
            newPos.x = pos.x + m_Direction.x * m_Speed * real::Time::GetInstance().GetElapsed();
    }

    // if owner overlaps with stair
    for (const auto& stair : m_StairPtrs)
    {
        if (stair->HasComponent<real::ColliderComponent>() == false)
            continue;

        if (stair->GetComponent<real::ColliderComponent>()->IsEntireColliderOverlapping(futureCollider))
            newPos.y = pos.y + m_Direction.y * m_Speed * real::Time::GetInstance().GetElapsed();
    }

    GetOwner()->GetComponent<PlayerCharacter>()->SetDirection(m_Direction);

    // Translate the TransformComponent to the new position.
    //pTransform->Translate(newPos);
    pTransform->SetWorldPosition(newPos);
}
