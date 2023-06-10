#include "ItemSpawnerPrefab.h"

#include <Scene.h>
#include <TransformComponent.h>
#include <SpriteComponent.h>
#include <ResourceManager.h>
#include <ColliderComponent.h>

#include "GameInfo.h"
#include "ItemSpawner.h"


real::GameObject* ItemSpawnerPrefab::CreateItemSpawnerPrefab(real::Scene* pScene, const glm::vec2 pos)
{
	if (pScene == nullptr)
		return nullptr;

	const auto pGo = pScene->CreateGameObject();

	InitComponents(pGo, pos);

	return pGo;
}

real::GameObject* ItemSpawnerPrefab::CreateItemSpawnerPrefab(real::GameObject* pGameObject, const glm::vec2 pos)
{
	if (pGameObject == nullptr)
		return nullptr;

	const auto pGo = pGameObject->CreateGameObject();

	InitComponents(pGo, pos);

	return pGo;
}

void ItemSpawnerPrefab::InitComponents(real::GameObject* pOwner, const glm::vec2 pos)
{
	const auto pTexture = real::ResourceManager::GetInstance().LoadTexture("Misc/ItemsSpriteSheet.png");

	real::SpriteSheet spriteSheet;
	spriteSheet.pTexture = pTexture;
	spriteSheet.columns = 3;
	spriteSheet.rows = 1;
	spriteSheet.animTimer = 0;

	pOwner->AddComponent<real::SpriteComponent>(spriteSheet)->SetIsActive(false);
	const auto spriteSize = pOwner->GetComponent<real::SpriteComponent>()->GetSpriteSize();

	pOwner->GetComponent<real::TransformComponent>()->SetLocalPosition(pos.x, pos.y - spriteSize.y);
	pOwner->AddComponent<real::ColliderComponent>(spriteSize)->SetIsActive(false);
	pOwner->AddComponent<ItemSpawner>();
}
