#include "stdafx.h"
#include "IngredientPart.h"

#include <Scene.h>
#include <ColliderComponent.h>

//#include "GameInfo.h"
#include "TransformComponent.h"

void IngredientPart::Update()
{
	if (m_PlayerIsOnBurger == false)
		return;

	//const auto playerPtrs = real::SceneManager::GetInstance().GetActiveScene().FindObjectsWithTag(Tags::player);
	const auto collider = GetOwner()->GetComponent<real::ColliderComponent>();

	//for (const auto& player : playerPtrs)
	//{
		//const auto playerCollider = player->GetComponent<real::ColliderComponent>();

		if (collider->IsEntireColliderOverlapping(*m_Player))
		{
			m_IsTriggered = true;
			const auto translation = GetOwner()->GetComponent<real::TransformComponent>();
			translation->Translate(0, 3);
		}
	//}
}