#include "stdafx.h"
#include "Floor.h"

#include <Scene.h>
#include <ColliderComponent.h>

#include "TransformComponent.h"
#include "GameInfo.h"

real::GameObject* Floor::CreateFloor(real::Scene* pScene, const glm::vec2 pos, float width, bool drawDebug)
{
	if (pScene == nullptr)
		return nullptr;

	const auto pGo = pScene->CreateGameObject();

	InitComponents(pGo, pos, width, drawDebug);

	return pGo;
}

real::GameObject* Floor::CreateFloor(real::GameObject* pGameObject, const glm::vec2 pos, float width, bool drawDebug)
{
	if (pGameObject == nullptr)
		return nullptr;

	const auto pGo = pGameObject->CreateGameObject();

	InitComponents(pGo, pos, width, drawDebug);

	return pGo;
}

void Floor::InitComponents(real::GameObject* pOwner, const glm::vec2 pos, float width, bool drawDebug)
{
	constexpr float characterHeight = 48;
	
	pOwner->SetTag(Tags::floor);
	pOwner->GetComponent<real::TransformComponent>()->SetLocalPosition(pos.x, pos.y - characterHeight);
	pOwner->AddComponent<real::ColliderComponent>(glm::vec2{ width, characterHeight })->EnableDebugRendering(drawDebug, Colors::yellow);
}
