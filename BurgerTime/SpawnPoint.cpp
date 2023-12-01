#include "stdafx.h"
#include "SpawnPoint.h"

#include <Scene.h>
#include <TransformComponent.h>

#include "GameInfo.h"
#include "Logger.h"
#include "Spawner.h"

real::GameObject* SpawnPoint::CreateSpawnPoint(real::Scene* pScene, const glm::vec2 pos, const std::vector<std::string>& enemies)
{
	if (pScene == nullptr)
		return nullptr;

	const auto pGo = pScene->CreateGameObject();

	InitComponents(pGo, pos, enemies);

	return pGo;
}

real::GameObject* SpawnPoint::CreateSpawnPoint(real::GameObject* pGameObject, const glm::vec2 pos, const std::vector<std::string>& enemies)
{
	if (pGameObject == nullptr)
		return nullptr;

	const auto pGo = pGameObject->CreateGameObject();

	InitComponents(pGo, pos, enemies);

	return pGo;
}

void SpawnPoint::InitComponents(real::GameObject* pOwner, const glm::vec2 pos, const std::vector<std::string>& enemies)
{
	for (const auto& enemy : enemies)
	{
		if ((enemy == Tags::hot_dog || enemy == Tags::egg || enemy == Tags::pickle) == false)
		{
			real::Logger::LogWarning({"SpawnPoint prefab => {} was not recognized as a tag."}, enemy);
			return;
		}
	}

	//pOwner->SetTag(type);
	pOwner->GetComponent<real::TransformComponent>()->SetLocalPosition(pos);

	//if (type == Tags::hot_dog)
	//	pOwner->AddComponent<Spawner>(Spawner::EnemyTypes::hotDog)->SpawnEnemy();
	//else if (type == Tags::egg)
	//	pOwner->AddComponent<Spawner>(Spawner::EnemyTypes::egg)->SpawnEnemy();
	//else if (type == Tags::pickle)
	//	pOwner->AddComponent<Spawner>(Spawner::EnemyTypes::pickle)->SpawnEnemy();

	pOwner->AddComponent<Spawner>();

	for (const auto& enemy : enemies)
	{
		pOwner->GetComponent<Spawner>()->SpawnEnemy(enemy);
	}
}