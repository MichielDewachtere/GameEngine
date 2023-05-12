#include "stdafx.h"
#include "IngredientPrefab.h"

#include <Scene.h>
#include <ColliderComponent.h>
#include <TransformComponent.h>
#include <ResourceManager.h>
#include <Texture2D.h>
#include <TextureComponent.h>

#include "GameInfo.h"
#include "Ingredient.h"
#include "IngredientPart.h"


real::GameObject* IngredientPrefab::CreateIngredient(real::Scene* pScene, const std::string& texturePath, const glm::vec2 pos,
                                                     bool drawDebug)
{
	if (pScene == nullptr)
		return nullptr;

	const auto pGo = pScene->CreateGameObject();

	InitComponents(pGo, texturePath, pos, drawDebug);

	return pGo;
}

real::GameObject* IngredientPrefab::CreateIngredient(real::GameObject* pGameObject, const std::string& texturePath,
                                                     const glm::vec2 pos, bool drawDebug)
{
	if (pGameObject == nullptr)
		return nullptr;

	const auto pGo = pGameObject->CreateGameObject();

	InitComponents(pGo, texturePath, pos, drawDebug);

	return pGo;
}

void IngredientPrefab::InitComponents(real::GameObject* pOwner, const std::string& texturePath, const glm::vec2 pos, bool drawDebug)
{
	const auto leftTexture = real::ResourceManager::GetInstance().LoadTexture(texturePath + "left.png");
	const auto leftMiddleTexture = real::ResourceManager::GetInstance().LoadTexture(texturePath + "middle01.png");
	const auto leftRightTexture = real::ResourceManager::GetInstance().LoadTexture(texturePath + "middle02.png");
	const auto rightTexture = real::ResourceManager::GetInstance().LoadTexture(texturePath + "right.png");

	pOwner->SetTag(Tags::ingredient);
	pOwner->GetComponent<real::TransformComponent>()->SetLocalPosition(pos.x, pos.y - static_cast<float>(leftTexture->GetSize().y));
	const int ingredientWidth = leftTexture->GetSize().x * 4;
	pOwner->AddComponent<real::ColliderComponent>(glm::vec2{ ingredientWidth, 48 })->EnableDebugRendering(drawDebug, Colors::appelblauwzeegroen);
	pOwner->GetComponent<real::ColliderComponent>()->Translate(0, -33);
	pOwner->AddComponent<Ingredient>();

	const auto pIngredientLeft = pOwner->CreateGameObject();
	pIngredientLeft->AddComponent<real::TextureComponent>()->SetTexture(leftTexture);
	pIngredientLeft->AddComponent<real::ColliderComponent>(glm::vec2{ leftTexture->GetSize().x, 48 })->EnableDebugRendering(drawDebug, Colors::red);
	pIngredientLeft->GetComponent<real::ColliderComponent>()->Translate(0, -33);
	pIngredientLeft->AddComponent<IngredientPart>();

	const auto pIngredientMiddleLeft = pOwner->CreateGameObject();
	pIngredientMiddleLeft->GetComponent<real::TransformComponent>()->SetLocalPosition(static_cast<float>(leftMiddleTexture->GetSize().x), 0);
	pIngredientMiddleLeft->AddComponent<real::TextureComponent>()->SetTexture(leftMiddleTexture);
	pIngredientMiddleLeft->AddComponent<real::ColliderComponent>(glm::vec2{ leftMiddleTexture->GetSize().x, 48 })->EnableDebugRendering(drawDebug, Colors::red);
	pIngredientMiddleLeft->GetComponent<real::ColliderComponent>()->Translate(0, -33);
	pIngredientMiddleLeft->AddComponent<IngredientPart>();

	const auto pIngredientMiddleRight = pOwner->CreateGameObject();
	pIngredientMiddleRight->GetComponent<real::TransformComponent>()->SetLocalPosition(static_cast<float>(leftRightTexture->GetSize().x) * 2, 0);
	pIngredientMiddleRight->AddComponent<real::TextureComponent>()->SetTexture(leftRightTexture);
	pIngredientMiddleRight->AddComponent<real::ColliderComponent>(glm::vec2{ leftRightTexture->GetSize().x, 48 })->EnableDebugRendering(drawDebug, Colors::red);
	pIngredientMiddleRight->GetComponent<real::ColliderComponent>()->Translate(0, -33);
	pIngredientMiddleRight->AddComponent<IngredientPart>();

	const auto pIngredientRight = pOwner->CreateGameObject();
	pIngredientRight->GetComponent<real::TransformComponent>()->SetLocalPosition(static_cast<float>(rightTexture->GetSize().x) * 3, 0);
	pIngredientRight->AddComponent<real::TextureComponent>()->SetTexture(rightTexture);
	pIngredientRight->AddComponent<real::ColliderComponent>(glm::vec2{ rightTexture->GetSize().x, 48 })->EnableDebugRendering(drawDebug, Colors::red);
	pIngredientRight->GetComponent<real::ColliderComponent>()->Translate(0, -33);
	pIngredientRight->AddComponent<IngredientPart>();
}