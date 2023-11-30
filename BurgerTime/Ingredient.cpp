//#include "stdafx.h"
#include "Ingredient.h"

#include <Scene.h>
#include <ColliderComponent.h>
#include <GameTime.h>
#include <TextureComponent.h>
#include <TransformComponent.h>
#include <Locator.h>

#include "GameInfo.h"
#include "IngredientPart.h"

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
		//switch (m_PlatformsToSkip)
		//{
		//case 1:
		//	burgerDropped.Notify(500);
		//	break;
		//case 2:
		//	burgerDropped.Notify(1'000);
		//	break;
		//case 3:
		//	burgerDropped.Notify(2'000);
		//	break;
		//case 4:
		//	burgerDropped.Notify(4'000);
		//	break;
		//case 5:
		//	burgerDropped.Notify(8'000);
		//	break;
		//case 6:
		//	burgerDropped.Notify(16'000);
		//	break;
		//}

		//burgerDropped.Notify(50 * (m_PlatformsToSkip + 1));

		int score = 500;
		for (int i = 0; i < m_PlatformsToSkip; ++i)
		{
			score *= 2;
		}

		score += 50 * (m_PlatformsToSkip + 1);

		burgerDropped.Notify(score);

		real::Locator::GetAudioSystem().Play(Sounds::burger_falling);
		
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
	for (const auto& child : GetOwner()->GetChildren())
	{
		const auto pIngredientPart = child->GetComponent<IngredientPart>();
	
		pIngredientPart->Reset();
	}

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
			pOtherIngredient->GetComponent<Ingredient>()->SetIsFalling(true);
			// add another 50 points for the ingredient that is dropped by this ingredient
			burgerDropped.Notify(50);
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
				real::Locator::GetAudioSystem().Play(Sounds::burger_landing);
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

			landedOnPlate.Notify(*this);
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
