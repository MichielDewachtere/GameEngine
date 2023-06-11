//#include "stdafx.h"
#include "ScoreDisplay.h"

#include <SceneManager.h>
#include <TextComponent.h>

#include "GameInfo.h"
#include "Ingredient.h"
#include "BaseEnemy.h"
#include "PlayerCharacter.h"
#include "PlayerManager.h"

ScoreDisplay::ScoreDisplay(real::GameObject* pOwner)
	: Component(pOwner)
{
	real::SceneManager::GetInstance().onSceneSwitch.AddObserver(this);

	for (const auto& pPlayer : PlayerManager::GetInstance().GetPlayers())
	{
		pPlayer->GetComponent<PlayerCharacter>()->pickedUpItem.AddObserver(this);
	}
}

ScoreDisplay::~ScoreDisplay()
{
	real::SceneManager::GetInstance().onSceneSwitch.RemoveObserver(this);

	for (const auto& pPlayer : PlayerManager::GetInstance().GetPlayers())
	{
		pPlayer->GetComponent<PlayerCharacter>()->pickedUpItem.RemoveObserver(this);
	}
}

void ScoreDisplay::Start()
{
}

void ScoreDisplay::HandleEvent(int score)
{
	UpdateScoreText(score);
}

void ScoreDisplay::HandleEvent(real::Scene& scene, real::SceneManager::SceneSwitchState state)
{
	using SwitchState = real::SceneManager::SceneSwitchState;

	for (const auto& pIngredient : scene.FindObjectsWithTag(Tags::ingredient))
	{
		if (state == SwitchState::loaded)
		{
			pIngredient->GetComponent<Ingredient>()->burgerDropped.AddObserver(this);
		}
		else
		{
			pIngredient->GetComponent<Ingredient>()->burgerDropped.RemoveObserver(this);
		}
	}

	std::vector<real::GameObject*> pEnemies;

	for (const auto& pEnemy : scene.FindObjectsWithTag(Tags::hot_dog))
		pEnemies.push_back(pEnemy);
	for (const auto& pEnemy : scene.FindObjectsWithTag(Tags::pickle))
		pEnemies.push_back(pEnemy);
	for (const auto& pEnemy : scene.FindObjectsWithTag(Tags::egg))
		pEnemies.push_back(pEnemy);


	for (const auto& pEnemy : pEnemies)
	{
		if (state == SwitchState::loaded)
		{
			pEnemy->GetComponent<BaseEnemy>()->addScore.AddObserver(this);
		}
		else
		{
			pEnemy->GetComponent<BaseEnemy>()->addScore.RemoveObserver(this);
		}
	}
}

void ScoreDisplay::UpdateScoreText(int scoreToAdd)
{
	m_Score += scoreToAdd;
	scoreChanged.Notify(m_Score);
	PlayerManager::GetInstance().SetScore(m_Score);
	GetOwner()->GetComponent<real::TextComponent>()->SetText(std::to_string(m_Score));
}
