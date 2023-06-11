//#include "stdafx.h"
#include "IngredientPart.h"

#include <Scene.h>
#include <ColliderComponent.h>
#include <TransformComponent.h>
#include <Locator.h>

#include "GameInfo.h"
#include "Ingredient.h"

void IngredientPart::Update()
{
	if (m_PlayerIsOnBurger == false)
		return;

	const auto collider = GetOwner()->GetComponent<real::ColliderComponent>();

	if (collider->IsEntireColliderOverlapping(*m_Player) && m_IsTriggered == false)
	{
		m_IsTriggered = true;
		const auto translation = GetOwner()->GetComponent<real::TransformComponent>();
		translation->Translate(0, 3);

		const auto parent = GetOwner()->GetParent()->GetComponent<Ingredient>();
		parent->PartIsTriggered();

		real::Locator::GetAudioSystem().Play(Sounds::burger_part_activated);
	}
}

void IngredientPart::Reset()
{
	if (m_IsTriggered)
	{
		m_IsTriggered = false;
		const float yPos = GetOwner()->GetParent()->GetComponent<real::TransformComponent>()->GetWorldPosition().y;

		const auto worldPos = GetOwner()->GetComponent<real::TransformComponent>()->GetWorldPosition();

		GetOwner()->GetComponent<real::TransformComponent>()->SetWorldPosition(worldPos.x, yPos);
	}
}
