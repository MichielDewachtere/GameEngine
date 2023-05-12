#include "stdafx.h"
#include "Ingredient.h"

#include <Scene.h>
#include <ColliderComponent.h>
#include <Logger.h>
#include <GameTime.h>

#include "GameInfo.h"
#include "IngredientPart.h"
#include "TransformComponent.h"

void Ingredient::Start()
{
	//real::Logger::LogInfo("Called Ingredient Start function");
}

void Ingredient::Update()
{
	if (m_CurrentPlatform == 0)
		InitCurrentPlatform();

	if (m_IsFalling == false)
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
	else
	{
		Fall();
	}
}

void Ingredient::PartIsTriggered()
{
	constexpr int amountOfParts = 4;

	++m_AmountOfPartsTriggered;

	real::Logger::LogInfo("Part {} has been triggered.", m_AmountOfPartsTriggered);

	if (m_AmountOfPartsTriggered == amountOfParts)
	{
		real::Logger::LogInfo("All parts have been triggered");
		m_IsFalling = true;
		//TODO: this will depend on the weight/amount of enemies on burger.
		m_PlatformsToSkip = 1;
	}
}

void Ingredient::InitCurrentPlatform()
{
	const auto platformPtrs = real::SceneManager::GetInstance().GetActiveScene().FindObjectsWithTag(Tags::floor);
	const auto pCollider = GetOwner()->GetComponent<real::ColliderComponent>();

	for (const auto& pPlatform : platformPtrs)
	{
		const auto pPlatformCollider = pPlatform->GetComponent<real::ColliderComponent>();

		if (pPlatformCollider->IsEntireColliderOverlapping(*pCollider))
		{
			m_CurrentPlatform = pPlatform->GetId();
		}
	}
}

void Ingredient::ResetBurger()
{
	real::Logger::LogInfo("BurgerParts should move up");

	const auto transform = GetOwner()->GetComponent<real::TransformComponent>();

	for (const auto& child : GetOwner()->GetChildren())
	{
		const auto pChildTransform = child->GetComponent<real::TransformComponent>();
		const auto pChildCollider = child->GetComponent<real::ColliderComponent>();
		const auto pIngredientPart = child->GetComponent<IngredientPart>();

		pChildTransform->SetWorldPosition(pChildTransform->GetWorldPosition().x, transform->GetWorldPosition().y);
		pChildCollider->SetPosition(pChildTransform->GetWorldPosition());
		pIngredientPart->SetIsTriggered(false);
	}

	m_AmountOfPartsTriggered = 0;
	m_IsFalling = false;
}

void Ingredient::Fall()
{
	const auto pTransform = GetOwner()->GetComponent<real::TransformComponent>();
	const auto pCollider = GetOwner()->GetComponent<real::ColliderComponent>();
	const auto ingredientPtrs = real::SceneManager::GetInstance().GetActiveScene().FindObjectsWithTag(Tags::ingredient);
	const auto platformPtrs = real::SceneManager::GetInstance().GetActiveScene().FindObjectsWithTag(Tags::floor);

	const auto translation = glm::vec2{ 0, static_cast<float>(m_FallSpeed) * real::Time::GetInstance().GetElapsed() };
	pTransform->Translate(translation);

	for (const auto& pIngredient : ingredientPtrs)
	{
		const auto pIngredientCollider = pIngredient->GetComponent<real::ColliderComponent>();

		if (pCollider == pIngredientCollider)
			continue;

		if (pCollider->IsOverlapping(*pIngredientCollider))
		{
			pIngredient->GetComponent<real::TransformComponent>()->Translate({ 0,1 });
			pIngredient->GetComponent<Ingredient>()->SetIsFalling(true);
		}
	}

	for (const auto& pPlatform : platformPtrs)
	{
		const auto pPlatformCollider = pPlatform->GetComponent<real::ColliderComponent>();

		if (pPlatformCollider->IsEntireColliderOverlapping(*pCollider))
		{
			//++m_PlatformsCrossed;

			if (pPlatform->GetId() == m_CurrentPlatform)
				continue;

			m_CurrentPlatform = pPlatform->GetId();

			pTransform->Translate({ 0,-1 });

			//if (m_PlatformsCrossed > m_PlatformsToSkip)
			{
				real::Logger::LogInfo("Ingredient should stop falling");
				ResetBurger();
			}
		}
	}
}

void Ingredient::NotifyParts(bool playerOnBurger, real::ColliderComponent* pFeet) const
{
	for (const auto& child : GetOwner()->GetChildren())
	{
		const auto part = child->GetComponent<IngredientPart>();

		part->IsPlayerOnBurger(playerOnBurger, pFeet);
	}
}
