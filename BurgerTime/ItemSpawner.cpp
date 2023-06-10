﻿#include "ItemSpawner.h"

#include <GameTime.h>
#include <SpriteComponent.h>

#include "ColliderComponent.h"
#include "GameInfo.h"
#include "PlayerManager.h"

ItemSpawner::ItemSpawner(real::GameObject* pOwner)
	: Component(pOwner)
{

	//for (const auto& pIngredient : sceneManager.GetActiveScene().FindObjectsWithTag(Tags::ingredient))
	//{
	//	pIngredient->GetComponent<Ingredient>()->burgerDropped.AddObserver(this);
	//}
}

ItemSpawner::~ItemSpawner()
{
	if (GetOwner()->IsMarkedForDestroy() == false)
	{
		RemoveObservers();
	}
}

void ItemSpawner::Start()
{
	const int idx = PlayerManager::GetInstance().GetCurrentLevel();
	GetOwner()->GetComponent<real::SpriteComponent>()->SelectSprite(idx);

	auto& sceneManager = real::SceneManager::GetInstance();

	for (const auto& pIngredient : sceneManager.GetActiveScene().FindObjectsWithTag(Tags::ingredient))
	{
		pIngredient->GetComponent<Ingredient>()->burgerDropped.AddObserver(this);
	}

	sceneManager.onSceneExit.AddObserver(this);
}

void ItemSpawner::Update()
{
	if (m_IsActive == false)
		return;

	m_AccuTime += real::Time::GetInstance().GetElapsed();

	if (m_AccuTime >= m_LifeTime)
	{
		m_IsActive = false;
		SetComponentsActive(m_IsActive);
	}
}

void ItemSpawner::HandleEvent()
{
	m_IsActive = true;
	SetComponentsActive(m_IsActive);
}

void ItemSpawner::HandleEvent(real::Scene&)
{
	RemoveObservers();
}

void ItemSpawner::SetComponentsActive(bool enable)
{
	GetOwner()->GetComponent<real::SpriteComponent>()->SetIsActive(enable);
	GetOwner()->GetComponent<real::ColliderComponent>()->SetIsActive(enable);

	isItemSpawned.Notify(enable);
	m_AccuTime = 0;
}

void ItemSpawner::RemoveObservers()
{
	auto& sceneManager = real::SceneManager::GetInstance();

	sceneManager.onSceneExit.RemoveObserver(this);

	for (const auto& pIngredient : sceneManager.GetActiveScene().FindObjectsWithTag(Tags::ingredient))
	{
		pIngredient->GetComponent<Ingredient>()->burgerDropped.RemoveObserver(this);
	}
}