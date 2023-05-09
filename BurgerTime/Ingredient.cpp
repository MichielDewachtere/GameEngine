#include "stdafx.h"
#include "Ingredient.h"

#include <Scene.h>
#include <ColliderComponent.h>

#include "GameInfo.h"
#include "IngredientPart.h"

void Ingredient::Update()
{
	const auto playerPtrs = real::SceneManager::GetInstance().GetActiveScene().FindObjectsWithTag(Tags::player);
	const auto collider = GetOwner()->GetComponent<real::ColliderComponent>();

	for (const auto& player : playerPtrs)
	{
		const auto playerCollider = player->GetComponent<real::ColliderComponent>();
		const auto pFeetCollider = player->GetChildAt(0)->GetComponent<real::ColliderComponent>();

		if (collider->IsOverlapping(*playerCollider))
		{
			NotifyParts(true, pFeetCollider);
			break;
		}

		NotifyParts(false, nullptr);
	}
}

void Ingredient::PartIsTriggered()
{
	++m_AmountOfPartsTriggered;

	if (m_AmountOfPartsTriggered == 4)
	{
		Fall();
	} 
}

void Ingredient::Fall()
{
}

void Ingredient::NotifyParts(bool playerOnBurger, real::ColliderComponent* pFeet) const
{
	for (const auto& child : GetOwner()->GetChildren())
	{
		const auto part = child->GetComponent<IngredientPart>();

		part->IsPlayerOnBurger(playerOnBurger, pFeet);
	}
}
