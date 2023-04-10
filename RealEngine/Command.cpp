#include "stdafx.h"
#include "Command.h"

#include "GameObject.h"
#include "TransformComponent.h"

#include "HealthComponent.h"
#include "Input.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Time.h"

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

void real::TestCommand::Execute()
{
	std::cout << "test approved idk\n";
}

real::DamageCommand::DamageCommand(GameObject* object, const int damage)
    : Command(object)
    , m_Damage(damage)
{
}

void real::DamageCommand::Execute()
{
    const auto pHealthComponent = GetObject()->GetComponent<HealthComponent>();

    if (pHealthComponent == nullptr)
    {
        throw std::runtime_error("Error: Could not execute DamageCommand - no HealthComponent found on associated GameObject. Please ensure the GameObject has a HealthComponent attached.");
    }

    pHealthComponent->Damage(m_Damage);
}

real::AddPointsCommand::AddPointsCommand(GameObject* object, const int amount)
	: Command(object)
	, m_Amount(amount)
{
}

void real::AddPointsCommand::Execute()
{
	GetObject()->NotifyObservers(Observer::GameEvent::actorGainedPoints);
}

real::LoadNextSceneCommand::LoadNextSceneCommand(GameObject* object, std::string name)
	: Command(object)
	, m_Name(std::move(name))
{
}

void real::LoadNextSceneCommand::Execute()
{
    SceneManager::GetInstance().SetSceneActive(m_Name);
    Input::GetInstance().SetInputMapActive(SceneManager::GetInstance().GetActiveScene().GetDefaultInputMap());
}
