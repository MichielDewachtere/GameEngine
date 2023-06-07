//#include "stdafx.h"
#include "BaseEnemy.h"

#include <Scene.h>
#include <Logger.h>
#include <SceneManager.h>
#include <GameObject.h>
#include <GameTime.h>

#include "GameInfo.h"
#include "HealthComponent.h"
#include "Ingredient.h"
#include "PlayerCharacter.h"
#include "Spawner.h"
#include "SpriteComponent.h"

BaseEnemy::BaseEnemy(real::GameObject* pOwner)
	: Component(pOwner)
{
	real::SceneManager::GetInstance().onSceneExit.AddObserver(this);
}

BaseEnemy::~BaseEnemy()
{
	if (GetOwner()->IsMarkedForDestroy() == false)
	{
		for (const auto& pPlayer : m_PlayerPtrs)
		{
			pPlayer->GetComponent<PlayerCharacter>()->pepperThrown.RemoveObserver(this);
		}

		real::SceneManager::GetInstance().onSceneExit.RemoveObserver(this);
	}

}

void BaseEnemy::Start()
{
	for (const auto pPlayer: real::SceneManager::GetInstance().GetActiveScene().FindObjectsWithTag(Tags::player))
	{
		m_PlayerPtrs.push_back(/*std::unique_ptr<real::GameObject>*/(pPlayer));
	}

	for (const auto pStair : real::SceneManager::GetInstance().GetActiveScene().FindObjectsWithTag(Tags::stair))
	{
		m_StairPtrs.push_back(/*std::unique_ptr<real::GameObject>*/(pStair));
	}

	for (const auto pStair : real::SceneManager::GetInstance().GetActiveScene().FindObjectsWithTag(Tags::hidden_stair))
	{
		m_HiddenStairPtrs.push_back(/*std::unique_ptr<real::GameObject>*/(pStair));
	}

	for (const auto pFloor : real::SceneManager::GetInstance().GetActiveScene().FindObjectsWithTag(Tags::floor))
	{
		m_FloorPtrs.push_back(/*std::unique_ptr<real::GameObject>*/(pFloor));
	}

	for (const auto pIngredient : real::SceneManager::GetInstance().GetActiveScene().FindObjectsWithTag(Tags::ingredient))
	{
		m_IngredientPtrs.push_back(/*std::unique_ptr<real::GameObject>*/(pIngredient));
	}

	m_pWorldBorder = real::SceneManager::GetInstance().GetActiveScene().FindObjectsWithTag(Tags::boundary)[0]->GetComponent<real::ColliderComponent>();

	for (const auto& pPlayer : m_PlayerPtrs)
	{
		pPlayer->GetComponent<PlayerCharacter>()->pepperThrown.AddObserver(this);
	}
}

void BaseEnemy::Update()
{
	// check for targetted player
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
	const auto pCollider = GetOwner()->GetComponent<real::ColliderComponent>();
	const auto playerPos = pPlayerTransform->GetWorldPosition();
	const auto enemyPos = GetOwner()->GetComponent<real::TransformComponent>()->GetWorldPosition();

	switch (m_CurrentState)
	{
	case EnemyState::outOfBounds:
	{
		CheckForIngredients();

		//if (m_pWorldBorder->IsEntireColliderOverlapping(*pCollider))
		//	m_CurrentState = EnemyState::moveY;

		if (CheckForStairs(pPlayerTransform))
		{
			if (static_cast<int>(m_Direction.y) == 1)
			{
				GetOwner()->GetComponent<real::SpriteComponent>()->PlayAnimation(0, 2);
			}
			else if (static_cast<int>(m_Direction.y) == -1)
			{
				GetOwner()->GetComponent<real::SpriteComponent>()->PlayAnimation(4, 6);
			}

			m_CurrentState = EnemyState::moveY;
			MoveEnemy();
			break;
		}

		if (CheckForHiddenStairs())
		{
			MoveEnemy();
			break;
		}


		//if (CheckForHiddenStairs())
		//{
		//	if (static_cast<int>(m_Direction.y) == 1)
		//	{
		//		GetOwner()->GetComponent<real::SpriteComponent>()->PlayAnimation(4, 6);
		//	}
		//	else if (static_cast<int>(m_Direction.y) == -1)
		//	{
		//		GetOwner()->GetComponent<real::SpriteComponent>()->PlayAnimation(0, 2);
		//	}

 		//	m_CurrentState = EnemyState::moveY;
		//	MoveEnemy();
		//	break;
		//}


		if (playerPos.x < enemyPos.x && m_Direction == glm::vec2{0, 0})
		{
			if (GetOwner()->GetComponent<real::SpriteComponent>()->IsAnimationPlaying() == false)
			{
				GetOwner()->GetComponent<real::SpriteComponent>()->PlayAnimation(2, 4);
			}

			m_Direction = { -1,0 };
		}
		else if (m_Direction == glm::vec2{0, 0})
		{
			if (GetOwner()->GetComponent<real::SpriteComponent>()->IsAnimationPlaying() == false)
			{
				GetOwner()->GetComponent<real::SpriteComponent>()->PlayAnimation(2, 4);
				GetOwner()->GetComponent<real::SpriteComponent>()->FlipTexture(SDL_FLIP_HORIZONTAL);
			}
			m_Direction = { 1,0 };
		}

		MoveEnemy();

		break;
	}
	case EnemyState::moveX:
	{
		if (m_CheckForPepper)
			CheckForPepper();

		if (PlayerHit(pPlayer))
		{
			pPlayer->GetComponent<HealthComponent>()->Damage();
			// LevelManager -> reset level?
		}

		CheckForIngredients();

		if (CheckForStairs(pPlayerTransform))
		{
			if (static_cast<int>(m_Direction.y) == 1)
				GetOwner()->GetComponent<real::SpriteComponent>()->PlayAnimation(0, 2);
			else if (static_cast<int>(m_Direction.y) == -1)
				GetOwner()->GetComponent<real::SpriteComponent>()->PlayAnimation(4, 6);

			m_CurrentState = EnemyState::moveY;
			MoveEnemy();
			break;
		}

		const auto currentFloorCollider = real::SceneManager::GetInstance().GetActiveScene().FindObject(m_CurrentPlatform)->GetComponent<real::ColliderComponent>();

		// turn enemy around if it is gonna be out of bounds.
		if (m_Direction == glm::vec2{-1, 0}
			&& CanMoveTo(enemyPos, pCollider->GetSize(), *currentFloorCollider, Direction::left) == false
			&& WillHitStair() == false)
		{
			GetOwner()->GetComponent<real::SpriteComponent>()->FlipTexture(SDL_FLIP_HORIZONTAL);
			GetOwner()->GetComponent<real::SpriteComponent>()->PlayAnimation(2, 4);

			m_CurrentStair = 0;
			m_Direction = { 1,0 };
		}
		if (m_Direction == glm::vec2{1, 0}
			&& CanMoveTo(enemyPos, pCollider->GetSize(), *currentFloorCollider, Direction::right) == false
			&& WillHitStair() == false)
		{
			GetOwner()->GetComponent<real::SpriteComponent>()->PlayAnimation(2, 4);

			m_CurrentStair = 0;
			m_Direction = { -1,0 };
		}

		if (GetOwner()->GetComponent<real::SpriteComponent>()->IsAnimationPlaying() == false)
		{
			GetOwner()->GetComponent<real::SpriteComponent>()->PlayAnimation(2, 4);
			if (static_cast<int>(m_Direction.x) == 1)
				GetOwner()->GetComponent<real::SpriteComponent>()->FlipTexture(SDL_FLIP_HORIZONTAL);
		}

		MoveEnemy();
		break;
	}
	case EnemyState::moveY:
	{
		if (m_CheckForPepper)
			CheckForPepper();

		if (PlayerHit(pPlayer))
		{
			pPlayer->GetComponent<HealthComponent>()->Damage();
			// LevelManager -> reset level?
		}

		CheckForIngredients();

		if (GetOwner()->GetComponent<real::SpriteComponent>()->IsAnimationPlaying() == false)
		{
			if (static_cast<int>(m_Direction.y) == 1)
				GetOwner()->GetComponent<real::SpriteComponent>()->PlayAnimation(0, 2);
			else
				GetOwner()->GetComponent<real::SpriteComponent>()->PlayAnimation(4, 6);
		}

		if (m_CurrentStair != 0)
		{
			const auto pCurrentStairCollider = real::SceneManager::GetInstance().GetActiveScene().FindObject(m_CurrentStair)->GetComponent<real::ColliderComponent>();

			// if enemy goes up, check if it can continue go up if the player is higher then the enemy
			if (m_Direction == glm::vec2{0, -1})
			{
				if (CanMoveTo(enemyPos, pCollider->GetSize(), *pCurrentStairCollider, Direction::up)
					&& static_cast<int>(enemyPos.y) > static_cast<int>(playerPos.y))
				{
					MoveEnemy();
					break;
				}
			}
			else if (m_Direction == glm::vec2{0, 1})
			{
				if (CanMoveTo(enemyPos, pCollider->GetSize(), *pCurrentStairCollider, Direction::down)
					&& static_cast<int>(enemyPos.y) < static_cast<int>(playerPos.y))
				{
					MoveEnemy();
					break;
				}
			}
		}

		if (CheckForPlatforms(pPlayerTransform))
		{
			GetOwner()->GetComponent<real::SpriteComponent>()->PlayAnimation(2, 4);

			if (static_cast<int>(m_Direction.x) == 1)
				GetOwner()->GetComponent<real::SpriteComponent>()->FlipTexture(SDL_FLIP_HORIZONTAL);

			m_CurrentState = EnemyState::moveX;
			MoveEnemy();
			break;
		}

		const auto currentStairCollider = real::SceneManager::GetInstance().GetActiveScene().FindObject(m_CurrentStair)->GetComponent<real::ColliderComponent>();

		// turn enemy around if it is gonna be out of bounds.
		if (m_Direction == glm::vec2{ 0, 1 }
			&& CanMoveTo(enemyPos, pCollider->GetSize(), *currentStairCollider, Direction::down) == false
			&& WillHitPlatform() == false)
		{
			m_CurrentPlatform = 0;
			m_Direction = { 0, -1 };
		}
		if (m_Direction == glm::vec2{ 0, -1 }
			&& CanMoveTo(enemyPos, pCollider->GetSize(), *currentStairCollider, Direction::up) == false
			&& WillHitPlatform() == false)
		{
			m_CurrentPlatform = 0;
			m_Direction = { 0, 1 };
		}

		MoveEnemy();
		break;
	}
	case EnemyState::fall:
	{
		Fall();
		break;
	}
	case EnemyState::crushed:
	{
		//TODO: add points to score
		//real::Logger::LogInfo("BaseEnemy => Enemy {} is crushed", GetOwner()->GetId());

		m_DeathTimer += real::Time::GetInstance().GetElapsed();

		if (m_DeathTimer > m_MaxDeathTime)
		{
			m_DeathTimer = 0;

			GetOwner()->GetParent()->GetComponent<Spawner>()->ReSpawnEnemy(GetOwner());
			GetOwner()->GetComponent<real::SpriteComponent>()->Stop(true);

			m_Direction = glm::vec2{ 0,0 };
			m_CurrentStair = 0;
			m_CurrentPlatform = 0;
			m_CurrentState = EnemyState::outOfBounds;
		}

		break;
	}
	case EnemyState::stun:
	{
		m_StunTimer += real::Time::GetInstance().GetElapsed();

		if (m_StunTimer > m_MaxStunTime)
		{
			m_StunTimer = 0;

			GetOwner()->GetComponent<real::SpriteComponent>()->Stop(true);

			if (static_cast<int>(m_Direction.x) == 0)
				m_CurrentState = EnemyState::moveY;
			else
				m_CurrentState = EnemyState::moveX;
		}

		break;
	}
	case EnemyState::dead:
	{
		//TODO: add points to score

		GetOwner()->GetComponent<real::SpriteComponent>()->Stop(true);

		m_DeathTimer += real::Time::GetInstance().GetElapsed();

		if (m_DeathTimer > m_MaxDeathTime)
		{
			if (CheckForPlatforms(pPlayerTransform))
			{
				m_CurrentState = EnemyState::moveX;
			}
			else
			{
				m_Direction = glm::vec2{ 0, 0 };
				m_CurrentStair = 0;
				m_CurrentState = EnemyState::outOfBounds;
			}

			m_DeathTimer = 0;
		}

		break;
	}
	}
}

void BaseEnemy::HandleEvent(bool pepperActive)
{
	m_CheckForPepper = pepperActive;
}

void BaseEnemy::HandleEvent(real::Scene&)
{
	for (const auto& pPlayer : m_PlayerPtrs)
	{
		pPlayer->GetComponent<PlayerCharacter>()->pepperThrown.RemoveObserver(this);
	}

	real::SceneManager::GetInstance().onSceneExit.RemoveObserver(this);
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

		//m_IsOnStair = true;
		m_CurrentStair = pStair->GetId();

		//m_IsOnFloor = false;
		//m_CanTurn = true;

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

bool BaseEnemy::CheckForHiddenStairs()
{
	for (const auto& pStair : m_HiddenStairPtrs)
	{
		const auto pStairCollider = pStair->GetComponent<real::ColliderComponent>();
		const auto pCollider = GetOwner()->GetComponent<real::ColliderComponent>();

		if (pStairCollider->IsEntireColliderOverlapping(*pCollider))
		{
			if (m_Direction != glm::vec2{ 0,-1 })
			{
				GetOwner()->GetComponent<real::SpriteComponent>()->UpdateAnimation(4, 6);
				m_Direction = { 0,-1 };
			}

			return true;
		}
	}

	return false;
}

bool BaseEnemy::CheckForPlatforms(real::TransformComponent* playerTransform)
{
	const auto pCollider = GetOwner()->GetComponent<real::ColliderComponent>();

	for (const auto& pPlatform : m_FloorPtrs)
	{
		// if we are on the same platform, continue
		if (pPlatform->GetId() == m_CurrentPlatform)
			continue;

		const auto pPlatformCollider = pPlatform->GetComponent<real::ColliderComponent>();
		const auto enemyWorldPos = GetOwner()->GetComponent<real::TransformComponent>()->GetWorldPosition();

		bool canGoRight = CanMoveTo(enemyWorldPos, pCollider->GetSize(), *pPlatformCollider, Direction::right);
		bool canGoLeft = CanMoveTo(enemyWorldPos, pCollider->GetSize(), *pPlatformCollider, Direction::left);

		if (canGoRight == false && canGoLeft == false)
			continue;

		//m_IsOnFloor = true;
		m_CurrentPlatform = pPlatform->GetId();

		//m_IsOnStair = false;
		//m_CanTurn = true;

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
	//const auto pCollider = GetOwner()->GetComponent<real::ColliderComponent>();
	//const auto pSubCollider = std::make_unique<real::ColliderComponent>(this->GetOwner(), pCollider->GetSize().x / 2, pCollider->GetSize().y);
	//pSubCollider->Translate(pCollider->GetSize().x / 4, 0);
	const auto pSubCollider = GetOwner()->GetChildAt(0)->GetComponent<real::ColliderComponent>();

	if (m_IsOnIngredient == false || m_pCurrentIngredient->GetTag() != Tags::ingredient)
	{
		for (const auto& pIngredient : m_IngredientPtrs)
		{
			if (pIngredient->GetTag() != Tags::ingredient)
				continue;

			if (pIngredient->GetComponent<Ingredient>()->GetIsFalling())
				continue;

			for (const auto& pPart : pIngredient->GetChildren())
			{
				const auto pPartCollider = pPart->GetComponent<real::ColliderComponent>();
				if (pPartCollider->IsOverlapping(*pSubCollider))
				{
					m_pCurrentIngredient = pIngredient/*.get()*/;
					m_IsOnIngredient = true;

					m_pCurrentIngredient->GetComponent<Ingredient>()->IncreaseWeight();
					break;
				}

			}
		}
	}
	else
	{
		if (m_pCurrentIngredient->GetComponent<Ingredient>()->GetIsFalling() == true)
		{
			real::Logger::LogInfo("BaseEnemy => Enemy {} should fall with burger", GetOwner()->GetId());
			GetOwner()->GetComponent<real::SpriteComponent>()->Pause(true);
			m_CurrentState = EnemyState::fall;
			return;
		}

		//const auto pIngredientCollider = m_pCurrentIngredient->GetComponent<real::ColliderComponent>();

		//if (pIngredientCollider->IsOverlapping(*pSubCollider) == false)
		//{
		//	m_pCurrentIngredient->GetComponent<Ingredient>()->DecreaseWeight();

		//	m_pCurrentIngredient = nullptr;
		//	m_IsOnIngredient = false;
		//}

		bool isOnBurger = false;
		for (const auto& pPart : m_pCurrentIngredient->GetChildren())
		{
			const auto pPartCollider = pPart->GetComponent<real::ColliderComponent>();
			if (pPartCollider->IsOverlapping(*pSubCollider) /*== false*/)
			{
				isOnBurger = true;
				break;
			}
		}

		if (isOnBurger == false)
		{
			m_pCurrentIngredient->GetComponent<Ingredient>()->DecreaseWeight();
			m_pCurrentIngredient = nullptr;
			m_IsOnIngredient = false;
		}
	}

	if (m_CurrentState == EnemyState::fall)
		return;

	for (const auto& pIngredient : m_IngredientPtrs)
	{
		if (pIngredient->GetTag() != Tags::ingredient)
			continue;

		const auto pIngredientTransform = pIngredient->GetComponent<real::TransformComponent>();
		if (pIngredientTransform->GetWorldPosition().y > GetOwner()->GetComponent<real::TransformComponent>()->GetWorldPosition().y)
			return;

		const auto pIngredientComponent = pIngredient->GetComponent<Ingredient>();

		if (pIngredientComponent->GetIsFalling() == false)
			continue;

		//real::Logger::LogInfo("Burger is falling");

		const auto pIngredientCollider = pIngredient->GetComponent<real::ColliderComponent>();
		const auto pCoreCollider = GetOwner()->GetChildAt(0)->GetComponent<real::ColliderComponent>();
		
		if (pIngredientCollider->IsOverlapping(*pCoreCollider))
		{
			GetOwner()->GetComponent<real::SpriteComponent>()->PlayAnimation(6, 10, 0);
			m_CurrentState = EnemyState::crushed;
			return;
		}
	}
}

void BaseEnemy::CheckForPepper()
{
	const auto pepperPtrs = real::SceneManager::GetInstance().GetActiveScene().FindObjectsWithTag(Tags::pepper);

	for (const auto& pPepper : pepperPtrs)
	{
		const auto pEnemyCollider = GetOwner()->GetComponent<real::ColliderComponent>();
		const auto pPepperCollider = pPepper->GetComponent<real::ColliderComponent>();

		if (pPepperCollider->IsOverlapping(*pEnemyCollider))
		{
			GetOwner()->GetComponent<real::SpriteComponent>()->PlayAnimation(10, 12, -1);
			m_CurrentState = EnemyState::stun;
			return;
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
		//m_IsFalling = false;
		m_CurrentState = EnemyState::dead;

		//const auto it = std::ranges::find_if(m_IngredientPtrs, [&](const std::unique_ptr<real::GameObject>& ptr) {
		//	return ptr.get() == m_pCurrentIngredient;
		//	});
		//
		//m_IngredientPtrs.erase(it);
		//
		
		//m_CurrentState = EnemyState::outOfBounds;
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
		m_CurrentState = EnemyState::dead;
	}
}

bool BaseEnemy::PlayerHit(real::GameObject* pPlayer) const
{
	const auto pPlayerFeetCollider = pPlayer->GetChildAt(0)->GetComponent<real::ColliderComponent>();
	const auto pEnemyCoreCollider = GetOwner()->GetChildAt(0)->GetComponent<real::ColliderComponent>();

	if (pPlayerFeetCollider->IsOverlapping(*pEnemyCoreCollider))
		return true;
	
	return false;
}

bool BaseEnemy::CanMoveTo(const glm::vec2& enemyPos, const glm::vec2& enemyColliderSize,
                          const real::ColliderComponent& structureCollider, Direction direction, bool entireCollider) const
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

	if (entireCollider)
		return structureCollider.IsEntireColliderOverlapping(*pFutureCollider);

	return structureCollider.IsOverlapping(*pFutureCollider);
}

bool BaseEnemy::IsInBounds() const
{
	const auto pCollider = GetOwner()->GetComponent<real::ColliderComponent>();
	return m_pWorldBorder->IsEntireColliderOverlapping(*pCollider);
}

bool BaseEnemy::WillHitPlatform() const
{
	const auto pEnemyCollider = GetOwner()->GetComponent<real::ColliderComponent>();

	for (const auto& pFloor : m_FloorPtrs)
	{
		const auto pFloorCollider = pFloor->GetComponent<real::ColliderComponent>();

		if (pFloorCollider->IsOverlapping(*pEnemyCollider))
			return true;
	}

	return false;
}

bool BaseEnemy::WillHitStair() const
{
	const auto pEnemyCollider = GetOwner()->GetComponent<real::ColliderComponent>();

	for (const auto& pStair : m_StairPtrs)
	{
		const auto pStairCollider = pStair->GetComponent<real::ColliderComponent>();

		if (pStairCollider->IsOverlapping(*pEnemyCollider))
			return true;
	}

	return false;

}
