//#include "stdafx.h"
#include "PlayerCharacter.h"

#include <GameTime.h>
#include <ColliderComponent.h>
#include <Scene.h>
#include <TextureComponent.h>
#include <TransformComponent.h>
#include <Locator.h>

#include "GameInfo.h"
#include "ItemSpawner.h"
#include "PlayerManager.h"

PlayerCharacter::PlayerCharacter(real::GameObject* pOwner)
	: Component(pOwner)
{
}

PlayerCharacter::~PlayerCharacter()
{
	real::SceneManager::GetInstance().onSceneSwitch.RemoveObserver(this);

	if (PlayerManager::GetInstance().GetAmountOfPlayers() == 1)
		return;

	for (const auto& pPlayer : PlayerManager::GetInstance().GetPlayers())
	{
		if (pPlayer->GetId() == GetOwner()->GetId())
			continue;

		pPlayer->GetComponent<PlayerCharacter>()->statsChanged.RemoveObserver(this);
	}
}

void PlayerCharacter::Start()
{
	real::SceneManager::GetInstance().onSceneSwitch.AddObserver(this);

	if (PlayerManager::GetInstance().GetAmountOfPlayers() == 1)
		return;

	for (const auto& pPlayer : PlayerManager::GetInstance().GetPlayers())
	{
		if (pPlayer->GetId() == GetOwner()->GetId())
			continue;

		pPlayer->GetComponent<PlayerCharacter>()->statsChanged.AddObserver(this);
	}
}

void PlayerCharacter::Update()
{
	if (m_PepperThrown)
	{
		m_AccuTime += real::Time::GetInstance().GetElapsed();

		if (m_AccuTime >= m_MaxStunTime)
		{
			m_AccuTime = 0;
			m_PepperThrown = false;
			GetOwner()->GetChildAt(1)->SetIsActive(false);
			pepperThrown.Notify(m_PepperThrown);
		}
	}

	if (m_ItemSpawned)
	{
		const auto pCoreCollider = GetOwner()->GetChildAt(0)->GetComponent<real::ColliderComponent>();
		const auto pItemCollider = m_pItem->GetComponent<real::ColliderComponent>();

		if (pCoreCollider->IsOverlapping(*pItemCollider))
		{
			int points = PlayerManager::GetInstance().GetCurrentLevel() % 3;

			if (points == 0)
				points = 3;

			++m_Peppers;

			real::Locator::GetAudioSystem().Play(Sounds::pepper_added);

			amountOfPepperChanged.Notify(m_Peppers);
			pickedUpItem.Notify(500 * points);

			m_ItemSpawned = false;
			m_pItem->GetComponent<ItemSpawner>()->SetComponentsActive(m_ItemSpawned);
		}
	}
}

void PlayerCharacter::HandleEvent(bool itemSpawned)
{
	m_ItemSpawned = itemSpawned;
}

void PlayerCharacter::HandleEvent(int stat, int newValue)
{
	if (stat == Stats::pepper)
	{
		m_Peppers = newValue;
		amountOfPepperChanged.Notify(m_Peppers);
	}
}

void PlayerCharacter::HandleEvent(real::Scene& scene, real::SceneManager::SceneSwitchState state)
{
	if (scene.GetName().find("Level") == std::string::npos)
		return;

	if (state == real::SceneManager::SceneSwitchState::loaded)
	{
		scene.FindObjectsWithTag(Tags::item_spawn)[0]->GetComponent<ItemSpawner>()->isItemSpawned.AddObserver(this);
		m_pItem = scene.FindObjectsWithTag(Tags::item_spawn)[0];
	}
	else if (state == real::SceneManager::SceneSwitchState::exit)
	{
		scene.FindObjectsWithTag(Tags::item_spawn)[0]->GetComponent<ItemSpawner>()->isItemSpawned.RemoveObserver(this);
		m_pItem = nullptr;
	}
}

void PlayerCharacter::ThrowPepper()
{
	if (m_PepperThrown || m_Peppers <= 0)
		return;

	m_PepperThrown = true;
	pepperThrown.Notify(m_PepperThrown);

	--m_Peppers;
	amountOfPepperChanged.Notify(m_Peppers);
	statsChanged.Notify(Stats::pepper, m_Peppers);

	real::Locator::GetAudioSystem().Play(Sounds::pepper_thrown);

	const auto pPepperTransform = GetOwner()->GetChildAt(1)->GetComponent<real::TransformComponent>();
	const auto playerPos = GetOwner()->GetComponent<real::TransformComponent>()->GetWorldPosition();

	const auto pepperSize = GetOwner()->GetChildAt(1)->GetComponent<real::TextureComponent>()->GetTexture()->GetSize();

	const auto newPos = glm::vec2{ static_cast<float>(pepperSize.x) * m_CurrentDirection.x,
		 static_cast<float>(pepperSize.y) * m_CurrentDirection.y } + playerPos;
	pPepperTransform->SetLocalPosition(newPos);

	GetOwner()->GetChildAt(1)->SetIsActive(true);
}
