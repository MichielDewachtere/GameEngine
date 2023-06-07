//#include "stdafx.h"
#include "Ingredient.h"

#include <Scene.h>
#include <ColliderComponent.h>
#include <GameTime.h>
#include <TextureComponent.h>
#include <TransformComponent.h>

#include "GameInfo.h"
#include "IngredientPart.h"

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

	if (m_AmountOfPartsTriggered == amountOfParts)
	{
		m_IsFalling = true;
	}
}

void Ingredient::InitCurrentPlatform()
{
	const auto platformPtrs = real::SceneManager::GetInstance().GetActiveScene().FindObjectsWithTag(Tags::floor);
	const auto pCollider = GetOwner()->GetChildAt(0)->GetComponent<real::ColliderComponent>();

	for (const auto& pPlatform : platformPtrs)
	{
		const auto pPlatformCollider = pPlatform->GetComponent<real::ColliderComponent>();

		if (pPlatformCollider->IsEntireColliderOverlapping(*pCollider))
		{
			m_CurrentPlatform = pPlatform->GetId();
			return;
		}
	}
}

void Ingredient::ResetBurger()
{
	//real::Logger::LogInfo("BurgerParts should move up");

	//const auto transform = GetOwner()->GetComponent<real::TransformComponent>();

	for (const auto& child : GetOwner()->GetChildren())
	{
		const auto pIngredientPart = child->GetComponent<IngredientPart>();
	
		pIngredientPart->Reset();
	}

	//transform->Translate(0, 3);

	m_PlatformsCrossed = 0;
	m_AmountOfPartsTriggered = 0;
	m_PlatformsToSkip = 0;
	m_IsFalling = false;
}

void Ingredient::Fall()
{
	const auto pTransform = GetOwner()->GetComponent<real::TransformComponent>();

	const auto translation = glm::vec2{ 0, static_cast<float>(m_FallSpeed) * real::Time::GetInstance().GetElapsed() };
	pTransform->Translate(translation);

	const auto pIngredientCollider = GetOwner()->GetComponent<real::ColliderComponent>();
	const auto pPartCollider = GetOwner()->GetChildAt(0)->GetComponent<real::ColliderComponent>();
	const auto ingredientPtrs = real::SceneManager::GetInstance().GetActiveScene().FindObjectsWithTag(Tags::ingredient);
	const auto platformPtrs = real::SceneManager::GetInstance().GetActiveScene().FindObjectsWithTag(Tags::floor);
	const auto platePtrs = real::SceneManager::GetInstance().GetActiveScene().FindObjectsWithTag(Tags::plate);

	// if ingredientCollider overlaps with Ingredient collider => other ingredient should fall.
	for (const auto& pOtherIngredient : ingredientPtrs)
	{
		if (pOtherIngredient->HasComponent<Ingredient>() == false)
			continue;

		if (pOtherIngredient->GetComponent<Ingredient>()->GetIsFalling())
			continue;

		if (GetOwner()->GetId() == pOtherIngredient->GetId())
			continue;

		if (pOtherIngredient->GetComponent<real::TransformComponent>()->GetWorldPosition().y < pTransform->GetWorldPosition().y)
			continue;

		const auto pIngredientOtherCollider = pOtherIngredient->GetComponent<real::ColliderComponent>();

		if (pIngredientCollider->IsOverlapping(*pIngredientOtherCollider))
		{
			//pOtherIngredient->GetComponent<real::TransformComponent>()->Translate({ 0,6 });
			pOtherIngredient->GetComponent<Ingredient>()->SetIsFalling(true);
		}
	}

	// if ingredientPart collider overlaps with platform collider => ingredient is on platform & should stop moving
	for (const auto& pPlatform : platformPtrs)
	{
		if (pPlatform->GetId() == m_CurrentPlatform)
			continue;

		const auto pPlatformCollider = pPlatform->GetComponent<real::ColliderComponent>();

		if (pPlatformCollider->IsEntireColliderOverlapping(*pPartCollider, { 0,1 }))
		{
			m_CurrentPlatform = pPlatform->GetId();

			if (m_PlatformsCrossed > m_PlatformsToSkip)
			{
				pTransform->SetWorldPosition({ pTransform->GetWorldPosition().x, pPlatformCollider->GetPosition().y + 33 });
				//pTransform->Translate({ 0,-1 });
				ResetBurger();
			}

			++m_PlatformsCrossed;
			return;
		}
	}

	// if ingredientCollider overlaps with plate, this should get destroyed 
	for (const auto& pPlate : platePtrs)
	{
		const auto pPlateCollider = pPlate->GetComponent<real::ColliderComponent>();

		if (pIngredientCollider->IsOverlapping(*pPlateCollider))
		{
			ResetBurger();

			constexpr float offset = 3;

			const auto pPlateTransform = pPlate->GetComponent<real::TransformComponent>();
			const auto size = GetOwner()->GetChildAt(0)->GetComponent<real::TextureComponent>()->GetTexture()->GetSize();
			pPlateTransform->Translate({ 0, static_cast<float>(-size.y) - offset });

			GetOwner()->SetTag(Tags::empty);
			GetOwner()->RemoveComponent<Ingredient>();
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
