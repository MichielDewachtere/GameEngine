//#include "stdafx.h"
#include "HealthComponent.h"

#include <GameTime.h>
#include <SpriteComponent.h>
#include <SceneManager.h>
#include <TransformComponent.h>

#include "PlayerManager.h"
#include "BaseEnemy.h"
#include "GameInfo.h"
#include "PlayerCharacter.h"

HealthComponent::HealthComponent(real::GameObject* pOwner, int lives)
	: Component(pOwner)
	, m_Lives(lives)
{
}

HealthComponent::~HealthComponent()
{
	if (PlayerManager::GetInstance().GetAmountOfPlayers() == 1)
		return;

	for (const auto& pPlayer : PlayerManager::GetInstance().GetPlayers())
	{
		if (pPlayer->GetId() == GetOwner()->GetId())
			continue;

		pPlayer->GetComponent<HealthComponent>()->onStatChanged.RemoveObserver(this);
	}
}

void HealthComponent::Start()
{
	if (PlayerManager::GetInstance().GetAmountOfPlayers() == 1)
		return;

	const auto playerPtrs = PlayerManager::GetInstance().GetPlayers();
	for (const auto& pPlayer : playerPtrs)
	{
		if (pPlayer->GetId() == GetOwner()->GetId())
			continue;

		pPlayer->GetComponent<HealthComponent>()->onStatChanged.AddObserver(this);
	}
}

void HealthComponent::Update()
{
	if (m_PlayerDied)
	{
		m_AccuTime += real::Time::GetInstance().GetElapsed();

		const auto spriteComponent = GetOwner()->GetComponent<real::SpriteComponent>();

		if (m_AccuTime >= 1 && m_SpriteChanged == false)
		{
			m_SpriteChanged = true;
			spriteComponent->PlayAnimation(14, 16, 0);
		}

		if (m_SpriteChanged)
		{
			if (spriteComponent->IsAnimationPlaying() == false)
				spriteComponent->PlayAnimation(16, 18);

			if (m_AccuTime >= m_TimeToRespawn)
			{
				for (const auto& pEnemy : m_EnemyPtrs)
				{
					pEnemy->GetOwner()->GetComponent<real::SpriteComponent>()->Stop(true);
					pEnemy->RespawnEnemy();
				}

				m_AccuTime = 0;
				m_SpriteChanged = false;
				m_PlayerDied = false;
				
				Respawn();
			}
		}
	}
}

void HealthComponent::HandleEvent(int stat, int health)
{
	if (stat == PlayerCharacter::Stats::health)
	{
		m_PlayerDied = true;
		GetOwner()->GetComponent<real::SpriteComponent>()->SelectSprite(13);
		m_Lives = health;
	}
}

void HealthComponent::Damage()
{
	--m_Lives;
	m_PlayerDied = true;
	onStatChanged.Notify(PlayerCharacter::Stats::health, m_Lives);
	GetOwner()->GetComponent<real::SpriteComponent>()->SelectSprite(13);

	const auto& scene = real::SceneManager::GetInstance().GetActiveScene();

	m_EnemyPtrs.clear();
	for (const auto& pEnemy : scene.FindObjectsWithTag(Tags::hot_dog))
	{
		const auto pBaseEnemy = pEnemy->GetComponent<BaseEnemy>();
		m_EnemyPtrs.push_back(pBaseEnemy);
		pBaseEnemy->PauseEnemy();
	}
	for (const auto& pEnemy : scene.FindObjectsWithTag(Tags::pickle))
	{
		const auto pBaseEnemy = pEnemy->GetComponent<BaseEnemy>();
		m_EnemyPtrs.push_back(pBaseEnemy);
		pBaseEnemy->PauseEnemy();
	}
	for (const auto& pEnemy : scene.FindObjectsWithTag(Tags::egg))
	{
		const auto pBaseEnemy = pEnemy->GetComponent<BaseEnemy>();
		m_EnemyPtrs.push_back(pBaseEnemy);
		pBaseEnemy->PauseEnemy();
	}

	if (m_Lives == 0)
	{
		real::SceneManager::GetInstance().SetSceneActive(Scenes::game_over_menu, m_TimeToRespawn);
	}
	else
	{
		playerDied.Notify();
	}
}

void HealthComponent::Respawn() const
{
	const auto transformComponent = this->GetOwner()->GetComponent<real::TransformComponent>();

	if (transformComponent == nullptr)
		return;

	GetOwner()->GetComponent<real::SpriteComponent>()->SelectSprite(1);

	transformComponent->SetWorldPosition(m_SpawnPoint);
}
