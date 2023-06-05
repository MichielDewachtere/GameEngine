//#include "stdafx.h"
#include "DamageCommand.h"

#include "HealthComponent.h"

DamageCommand::DamageCommand(real::GameObject* object, const int damage)
    : Command(object)
    , m_Damage(damage)
{
}

void DamageCommand::Execute()
{
    using namespace real;

    const auto pHealthComponent = GetOwner()->GetComponent<HealthComponent>();

    if (pHealthComponent == nullptr)
    {
        throw std::runtime_error("Error: Could not execute DamageCommand - no HealthComponent found on associated GameObject. Please ensure the GameObject has a HealthComponent attached.");
    }

    pHealthComponent->Damage();
}
