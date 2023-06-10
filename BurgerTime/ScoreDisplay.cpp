//#include "stdafx.h"
#include "ScoreDisplay.h"

//#include <Scene.h>
#include <SceneManager.h>
#include <TextComponent.h>

#include "GameInfo.h"
#include "Ingredient.h"
#include "BaseEnemy.h"

ScoreDisplay::ScoreDisplay(real::GameObject* pOwner)
	: Component(pOwner)
{
	//real::SceneManager::GetInstance().onSceneLoaded.AddObserver(this);
	real::SceneManager::GetInstance().onSceneExit.AddObserver(this);
}

ScoreDisplay::~ScoreDisplay()
{
	if (real::SceneManager::GetInstance().onSceneExit.HasObserver(this))
		real::SceneManager::GetInstance().onSceneExit.RemoveObserver(this);
	else
		real::SceneManager::GetInstance().onSceneLoaded.RemoveObserver(this);
}

void ScoreDisplay::Start()
{
}

void ScoreDisplay::HandleEvent(int score)
{
	UpdateScoreText(score);
}

void ScoreDisplay::HandleEvent(real::Scene& scene)
{
	for (const auto& pIngredient	 : scene.FindObjectsWithTag(Tags::ingredient))
	{
		if (pIngredient->GetComponent<Ingredient>()->burgerDropped.HasObserver(this) == false)
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
		if (pEnemy->GetComponent<BaseEnemy>()->addScore.HasObserver(this) == false)
		{
			pEnemy->GetComponent<BaseEnemy>()->addScore.AddObserver(this);
		}
		else
		{
			pEnemy->GetComponent<BaseEnemy>()->addScore.RemoveObserver(this);
		}
	}

	if (real::SceneManager::GetInstance().onSceneExit.HasObserver(this))
	{
		real::SceneManager::GetInstance().onSceneExit.RemoveObserver(this);
		real::SceneManager::GetInstance().onSceneLoaded.AddObserver(this);
	}
	else if (real::SceneManager::GetInstance().onSceneLoaded.HasObserver(this))
	{
		real::SceneManager::GetInstance().onSceneLoaded.RemoveObserver(this);
		real::SceneManager::GetInstance().onSceneExit.AddObserver(this);
	}
}

void ScoreDisplay::UpdateScoreText(int scoreToAdd)
{
	m_Score += scoreToAdd;
	scoreChanged.Notify(m_Score);
	GetOwner()->GetComponent<real::TextComponent>()->SetText(std::to_string(m_Score));
}
