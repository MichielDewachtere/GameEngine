//#include "stdafx.h"
#include "LivesDisplay.h"

#include <TextComponent.h>
#include <TransformComponent.h>
#include <ResourceManager.h>

#include "HealthComponent.h"
#include "PlayerManager.h"
#include "ScoreDisplay.h"

LivesDisplay::LivesDisplay(real::GameObject* pOwner)
	: Component(pOwner)
{
	const auto players = PlayerManager::GetInstance().GetPlayers();

	for (const auto& pPlayer : players)
	{
		pPlayer->GetComponent<HealthComponent>()->playerDied.AddObserver(this);
	}

	m_AmountOfPlayers = static_cast<int>(players.size());
	InitLifeIcons(m_AmountOfPlayers);
}

LivesDisplay::~LivesDisplay()
{
	if (m_SubjectDestroyed == false)
		PlayerManager::GetInstance().GetHUD()->GetComponentInChildren<ScoreDisplay>()->AddLife.RemoveObserver(this);

	const auto players = PlayerManager::GetInstance().GetPlayers();

	for (const auto& pPlayer : players)
	{
		pPlayer->GetComponent<HealthComponent>()->playerDied.RemoveObserver(this);
	}
}

void LivesDisplay::Start()
{
	PlayerManager::GetInstance().GetHUD()->GetComponentInChildren<ScoreDisplay>()->AddLife.AddObserver(this);
}

void LivesDisplay::HandleEvent()
{
	const auto pLiveObject = GetOwner()->CreateGameObject();
	pLiveObject->AddComponent<real::SpriteComponent>(m_SpriteSheet)->SelectSprite(m_AmountOfPlayers - 1);
	const auto spriteSize = pLiveObject->GetComponent<real::SpriteComponent>()->GetSpriteSize();
	pLiveObject->GetComponent<real::TransformComponent>()->Translate(0, -(spriteSize.y + 3) * static_cast<float>(m_LifeIcons));

	++m_LifeIcons;
}

void LivesDisplay::HandleEvent(bool value)
{
	if (value == true)
	{
		const auto children = GetOwner()->GetChildren();
		children.back()->Destroy();
		
		--m_LifeIcons;
	}
}

void LivesDisplay::InitLifeIcons(int players)
{
	const auto pTexture = real::ResourceManager::GetInstance().LoadTexture("misc/iconsspritesheet.png");

	m_SpriteSheet.pTexture = pTexture;
	m_SpriteSheet.columns = 6;
	m_SpriteSheet.rows = 1;
	m_SpriteSheet.animTimer = 0;

	constexpr int livesIcons = 3;
	for (int i = 0; i < livesIcons; ++i)
	{
		const auto pLiveObject = GetOwner()->CreateGameObject();
		pLiveObject->AddComponent<real::SpriteComponent>(m_SpriteSheet)->SelectSprite(players - 1);
		const auto spriteSize = pLiveObject->GetComponent<real::SpriteComponent>()->GetSpriteSize();
		pLiveObject->GetComponent<real::TransformComponent>()->Translate(0, -(spriteSize.y + 3) * static_cast<float>(i));
	}
}
