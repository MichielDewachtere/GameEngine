//#include "stdafx.h"
#include "BaseEnemy.h"

#include <Scene.h>
#include <Logger.h>
#include <SceneManager.h>
#include <GameObject.h>
#include <TransformComponent.h>
#include <ColliderComponent.h>
#include <GameTime.h>

#include "GameInfo.h"
#include "HealthComponent.h"
#include "Ingredient.h"
#include "Spawner.h"

void BaseEnemy::Start()
{
	for (const auto pPlayer: real::SceneManager::GetInstance().GetActiveScene().FindObjectsWithTag(Tags::player))
	{
		m_PlayerPtrs.push_back(std::unique_ptr<real::GameObject>(pPlayer));
	}

	for (const auto pStair : real::SceneManager::GetInstance().GetActiveScene().FindObjectsWithTag(Tags::stair))
	{
		m_StairPtrs.push_back(std::unique_ptr<real::GameObject>(pStair));
	}

	for (const auto pFloor : real::SceneManager::GetInstance().GetActiveScene().FindObjectsWithTag(Tags::floor))
	{
		m_FloorPtrs.push_back(std::unique_ptr<real::GameObject>(pFloor));
	}

	for (const auto pIngredient : real::SceneManager::GetInstance().GetActiveScene().FindObjectsWithTag(Tags::ingredient))
	{
		m_IngredientPtrs.push_back(std::unique_ptr<real::GameObject>(pIngredient));
	}
}

void BaseEnemy::Update()
{
	if (m_IsFalling)
	{
		Fall();
		return;
	}

	const auto playerPtrs = real::SceneManager::GetInstance().GetActiveScene().FindObjectsWithTag(Tags::player);
	real::GameObject* pPlayer{};

	// if multiplayer => search closest player
	if (playerPtrs.size() == 1)
		pPlayer = playerPtrs[0];
	else if (playerPtrs.size() > 1)
		pPlayer = GetClosestPlayer(playerPtrs);
	else if (playerPtrs.empty() || playerPtrs.size() > 2)
	{
		real::Logger::LogError("BaseEnemy => No players found");
		return;
	}

	const auto pPlayerTransform = pPlayer->GetComponent<real::TransformComponent>();

	if (PlayerHit(pPlayer))
	{
		pPlayer->GetComponent<HealthComponent>()->Damage();
		// LevelManager -> reset level?
	}

	CheckForIngredients();

	if (m_IsOnStair == false && m_IsOnFloor == false)
	{
		const auto playerPos = pPlayerTransform->GetWorldPosition();
		const auto enemyPos = GetOwner()->GetComponent<real::TransformComponent>()->GetWorldPosition();
		if (playerPos.x < enemyPos.x)
			m_Direction = { -1,0 };
		else
			m_Direction = { 1,0 };
	}

	if (m_IsOnStair == false)
	{
		if (CheckForStairs(pPlayerTransform))
		{
			MoveEnemy();
			return;
		}
	}

	if (m_IsOnFloor == false)
	{
		if (CheckForPlatforms(pPlayerTransform))
		{
			MoveEnemy();
			return;
		}
	}

	MoveEnemy();
}

real::GameObject* BaseEnemy::GetClosestPlayer(const std::vector<real::GameObject*>&)
{
	return nullptr;
}

bool BaseEnemy::CheckForStairs(real::TransformComponent* playerTransform)
{
	const auto pCollider = GetOwner()->GetComponent<real::ColliderComponent>();
	//const auto stairPtrs = real::SceneManager::GetInstance().GetActiveScene().FindObjectsWithTag(Tags::stair);

	for (const auto& pStair : m_StairPtrs)
	{
		if (pStair->GetId() == m_CurrentStair)
			continue;

		const auto pStairCollider = pStair->GetComponent<real::ColliderComponent>();
		const auto enemyWorldPos = GetOwner()->GetComponent<real::TransformComponent>()->GetWorldPosition();

		bool canGoUp = CanMoveTo(enemyWorldPos, pCollider->GetSize(), *pStairCollider, Direction::up);
		bool canGoDown = CanMoveTo(enemyWorldPos, pCollider->GetSize(), *pStairCollider, Direction::down);

		if (canGoUp == false && canGoDown == false)
			continue;

		m_IsOnStair = true;
		m_CurrentStair = pStair->GetId();

		m_IsOnFloor = false;
		m_CanTurn = true;

		if (canGoUp && canGoDown)
		{
			const auto pPlayerWorldPos = playerTransform->GetWorldPosition();
			if (pPlayerWorldPos.y > enemyWorldPos.y)
				canGoUp = false;
			else if (pPlayerWorldPos.y < enemyWorldPos.y)
				canGoDown = false;
			else
				std::rand() % 2 == 0 ? canGoUp = false : canGoDown = false;
		}

		if (canGoUp)
			m_Direction = { 0, -1 };
		else if (canGoDown)
			m_Direction = { 0,1 };

		return true;
	}

	return false;
}

bool BaseEnemy::CheckForPlatforms(real::TransformComponent* playerTransform)
{
	const auto pCollider = GetOwner()->GetComponent<real::ColliderComponent>();

	for (const auto& pPlatform : m_FloorPtrs)
	{
		if (pPlatform->GetId() == m_CurrentPlatform)
			continue;

		const auto pPlatformCollider = pPlatform->GetComponent<real::ColliderComponent>();
		const auto enemyWorldPos = GetOwner()->GetComponent<real::TransformComponent>()->GetWorldPosition();

		bool canGoRight = CanMoveTo(enemyWorldPos, pCollider->GetSize(), *pPlatformCollider, Direction::right);
		bool canGoLeft = CanMoveTo(enemyWorldPos, pCollider->GetSize(), *pPlatformCollider, Direction::left);

		if (canGoRight == false && canGoLeft == false)
			continue;

		m_IsOnFloor = true;
		m_CurrentPlatform = pPlatform->GetId();

		m_IsOnStair = false;
		m_CanTurn = true;

		if (canGoRight && canGoLeft)
		{
			const auto pPlayerWorldPos = playerTransform->GetWorldPosition();
			if (pPlayerWorldPos.x < enemyWorldPos.x)
				canGoRight = false;
			else if (pPlayerWorldPos.x > enemyWorldPos.x)
				canGoLeft = false;
			else
				std::rand() % 2 == 0 ? canGoRight = false : canGoLeft = false;
		}

		if (canGoRight)
			m_Direction = { 1,0 };
		else if (canGoLeft)
			m_Direction = { -1,0 };

		return true;
	}

	return false;
}

void BaseEnemy::CheckForIngredients()
{
	const auto pCollider = GetOwner()->GetComponent<real::ColliderComponent>();
	const auto pSubCollider = std::make_unique<real::ColliderComponent>(this->GetOwner(), pCollider->GetSize().x / 2, pCollider->GetSize().y);
	pSubCollider->Translate(pCollider->GetSize().x / 4, 0);

	if (m_IsOnIngredient == false || m_pCurrentIngredient->GetTag() != Tags::ingredient)
	{
		for (const auto& pIngredient : m_IngredientPtrs)
		{
			if (pIngredient->GetTag() != Tags::ingredient)
				continue;

			const auto pIngredientCollider = pIngredient->GetComponent<real::ColliderComponent>();

			if (pIngredientCollider->IsOverlapping(*pSubCollider))
			{
				m_pCurrentIngredient = pIngredient.get();
				m_IsOnIngredient = true;
			}
		}
	}
	else
	{
		if (m_pCurrentIngredient->GetComponent<Ingredient>()->GetIsFalling() == true)
		{
			real::Logger::LogInfo("BaseEnemy => Enemy {} should fall with burger", GetOwner()->GetId());
			m_IsFalling = true;
		}

		const auto pIngredientCollider = m_pCurrentIngredient->GetComponent<real::ColliderComponent>();

		if (pIngredientCollider->IsOverlapping(*pSubCollider) == false)
		{
			m_pCurrentIngredient = nullptr;
			m_IsOnIngredient = false;
		}
	}
}

void BaseEnemy::MoveEnemy()
{
	const glm::vec2 translation = real::Time::GetInstance().GetElapsed() * m_Direction * m_Speed;
	GetOwner()->GetComponent<real::TransformComponent>()->Translate(translation);
}

void BaseEnemy::Fall()
{
	if (m_pCurrentIngredient->HasComponent<Ingredient>() == false)
	{
		real::Logger::LogInfo("BaseEnemy => Enemy {} should be destroyed", GetOwner()->GetId());
		m_IsFalling = false;

		//const auto it = std::ranges::find_if(m_IngredientPtrs, [&](const std::unique_ptr<real::GameObject>& ptr) {
		//	return ptr.get() == m_pCurrentIngredient;
		//	});
		//
		//m_IngredientPtrs.erase(it);
		//
		m_pCurrentIngredient->SetTag(Tags::empty);

		//GetOwner()->Destroy();
		//GetOwner()->GetParent()->GetComponent<Spawner>()->SpawnEnemy();
		return;
	}

	const auto pIngredient = m_pCurrentIngredient->GetComponent<Ingredient>();
	if (pIngredient->GetIsFalling() == true)
	{
		const auto pTransform = GetOwner()->GetComponent<real::TransformComponent>();
		pTransform->Translate(0, pIngredient->GetFallSpeed() * real::Time::GetInstance().GetElapsed());
	}
	else
	{
		m_IsFalling = false;
	}
}

bool BaseEnemy::PlayerHit(real::GameObject* pPlayer) const
{
	const auto pPlayerCollider = pPlayer->GetComponent<real::ColliderComponent>();
	const auto pEnemyCollider = GetOwner()->GetComponent<real::ColliderComponent>();

	if (pPlayerCollider->IsOverlapping(*pEnemyCollider))
		return true;
	
	return false;
}

bool BaseEnemy::CanMoveTo(const glm::vec2& enemyPos, const glm::vec2& enemyColliderSize,
                          const real::ColliderComponent& structureCollider, Direction direction) const
{
	const auto pFutureCollider = std::make_unique<real::ColliderComponent>(nullptr, enemyColliderSize);

	switch (direction)
	{
	case Direction::up:
		pFutureCollider->SetPosition(enemyPos.x, enemyPos.y - 1);
		break;
	case Direction::down:
		pFutureCollider->SetPosition(enemyPos.x, enemyPos.y + 1);
		break;
	case Direction::left:
		pFutureCollider->SetPosition(enemyPos.x - 1, enemyPos.y);
		break;
	case Direction::right: 
		pFutureCollider->SetPosition(enemyPos.x + 1, enemyPos.y);
		break;
	}

	if (structureCollider.IsEntireColliderOverlapping(*pFutureCollider))
		return true;

	return false;
}
