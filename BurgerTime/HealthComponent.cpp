//#include "stdafx.h"
#include "HealthComponent.h"

#include <GameTime.h>
#include <SpriteComponent.h>
#include <SceneManager.h>
#include <TransformComponent.h>
#include <Locator.h>

#include "PlayerManager.h"
#include "BaseEnemy.h"
#include "GameInfo.h"
#include "PlayerCharacter.h"
#include "ScoreDisplay.h"

HealthComponent::HealthComponent(real::GameObject* pOwner)
	: Component(pOwner)
{
	real::SceneManager::GetInstance().onSceneLoaded.AddObserver(this);
}

HealthComponent::~HealthComponent()
{
	real::SceneManager::GetInstance().onSceneLoaded.RemoveObserver(this);
}

void HealthComponent::Start()
{
	PlayerManager::GetInstance().GetHUD()->GetComponentInChildren<ScoreDisplay>()->AddLife.AddObserver(this);
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

				real::Locator::GetAudioSystem().Play(Sounds::background);

				Respawn();
			}
		}
	}
}

void HealthComponent::HandleEvent()
{
	++m_Lives;
}

void HealthComponent::HandleEvent(real::Scene& scene)
{
	if (scene.GetName() == Scenes::game_over_menu)
	{
		if (PlayerManager::GetInstance().GetAmountOfPlayers() == 1 || PlayerManager::GetInstance().GetCurrentLevel() == 0)
			return;
	}
}

void HealthComponent::Damage()
{
	--m_Lives;
	m_PlayerDied = true;
	real::Locator::GetAudioSystem().Stop(Sounds::player_death.channel);
	real::Locator::GetAudioSystem().Play(Sounds::player_death);

	playerDied.Notify(true);

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
}

void HealthComponent::Respawn()
{
	const auto transformComponent = this->GetOwner()->GetComponent<real::TransformComponent>();

	if (transformComponent == nullptr)
		return;

	GetOwner()->GetComponent<real::SpriteComponent>()->SelectSprite(1);

	transformComponent->SetWorldPosition(m_SpawnPoint);

	playerDied.Notify(false);
}
