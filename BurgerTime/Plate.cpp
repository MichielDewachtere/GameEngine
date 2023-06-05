//#include "stdafx.h"
#include "Plate.h"

#include <Scene.h>
#include <ColliderComponent.h>
#include <TransformComponent.h>

#include "GameInfo.h"

real::GameObject* Plate::CreatePlate(real::Scene* pScene, const glm::vec2 pos, bool drawDebug)
{
	if (pScene == nullptr)
		return nullptr;

	const auto pGo = pScene->CreateGameObject();

	InitComponents(pGo, pos, drawDebug);

	return pGo;
}

real::GameObject* Plate::CreatePlate(real::GameObject* pGameObject, const glm::vec2 pos, bool drawDebug)
{
	if (pGameObject == nullptr)
		return nullptr;

	const auto pGo = pGameObject->CreateGameObject();

	InitComponents(pGo, pos, drawDebug);

	return pGo;
}

void Plate::InitComponents(real::GameObject* pOwner, const glm::vec2 pos, bool drawDebug)
{
	constexpr float plateHeight = 1;
	constexpr float plateWidth = 90;

	pOwner->SetTag(Tags::plate);
	pOwner->GetComponent<real::TransformComponent>()->SetLocalPosition(pos);
	pOwner->GetComponent<real::TransformComponent>()->Translate(0, -plateHeight);
	pOwner->AddComponent<real::ColliderComponent>(glm::vec2{ plateWidth, plateHeight })->EnableDebugRendering(drawDebug, Colors::green);
}
