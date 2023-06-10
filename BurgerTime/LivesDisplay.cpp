//#include "stdafx.h"
#include "LivesDisplay.h"

#include <TextComponent.h>
#include <TransformComponent.h>
#include <ResourceManager.h>
#include <SpriteComponent.h>

#include "HealthComponent.h"
#include "PlayerManager.h"

LivesDisplay::LivesDisplay(real::GameObject* pOwner)
	: Component(pOwner)
{
	const auto players = PlayerManager::GetInstance().GetPlayers();

	for (const auto& pPlayer : players)
	{
		pPlayer->GetComponent<HealthComponent>()->playerDied.AddObserver(this);
	}

	InitLifeIcons(static_cast<int>(players.size()));
}

LivesDisplay::~LivesDisplay()
{
	const auto players = PlayerManager::GetInstance().GetPlayers();

	for (const auto& pPlayer : players)
	{
		pPlayer->GetComponent<HealthComponent>()->playerDied.RemoveObserver(this);
	}
}

void LivesDisplay::HandleEvent()
{
	GetOwner()->GetChildAt(m_LivesLost)->SetIsActive(false);
	++m_LivesLost;
}

void LivesDisplay::InitLifeIcons(int players)
{
	const auto pTexture = real::ResourceManager::GetInstance().LoadTexture("misc/iconsspritesheet.png");

	real::SpriteSheet spriteSheet;
	spriteSheet.pTexture = pTexture;
	spriteSheet.columns = 6;
	spriteSheet.rows = 1;
	spriteSheet.animTimer = 0;

	constexpr int livesIcons = 3;
	for (int i = 0; i < livesIcons; ++i)
	{
		const auto pLiveObject = GetOwner()->CreateGameObject();
		pLiveObject->AddComponent<real::SpriteComponent>(spriteSheet)->SelectSprite(players - 1);
		const auto spriteSize = pLiveObject->GetComponent<real::SpriteComponent>()->GetSpriteSize();
		pLiveObject->GetComponent<real::TransformComponent>()->Translate(0, (spriteSize.y + 3) * i);
	}
}
