#include "stdafx.h"
#include "Stair.h"

#include <Scene.h>
#include <ColliderComponent.h>

#include "TransformComponent.h"
#include "GameInfo.h"

real::GameObject* Stair::CreateStair(real::Scene* pScene, const glm::vec2 pos, float height, bool drawDebug = false)
{
	if (pScene == nullptr)
		return nullptr;

	const auto pGo = pScene->CreateGameObject();

	InitComponents(pGo, pos, height, drawDebug);

	return pGo;
}

real::GameObject* Stair::CreateStair(real::GameObject* pGameObject, const glm::vec2 pos, float height, bool drawDebug = false)
{
	if (pGameObject == nullptr)
		return nullptr;

	const auto pGo = pGameObject->CreateGameObject();

	InitComponents(pGo, pos, height, drawDebug);

	return pGo;
}

void Stair::InitComponents(real::GameObject* pOwner, const glm::vec2 pos, float height, bool drawDebug)
{
	constexpr float characterHeight = 48;

	pOwner->SetTag(Tags::stair);
	pOwner->GetComponent<real::TransformComponent>()->SetLocalPosition(pos.x, pos.y - characterHeight);
	pOwner->AddComponent<real::ColliderComponent>(glm::vec2{ characterHeight, height + characterHeight })->EnableDebugRendering(drawDebug, Colors::purple);
}
