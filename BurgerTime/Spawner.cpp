//#include "stdafx.h"
#include "Spawner.h"

#include <GameTime.h>
#include <Logger.h>
#include <ResourceManager.h>
#include <TextureComponent.h>
#include <TransformComponent.h>
#include <ColliderComponent.h>

#include "BaseEnemy.h"
#include "GameInfo.h"

void Spawner::Update()
{
	if (m_NeedNewEnemy == false)
		return;

	const float dt = real::Time::GetInstance().GetElapsed();
	constexpr float spawnTime = 2;

	m_AccuTime += dt;

	if (m_AccuTime > spawnTime)
	{
		switch (m_Type)
		{
		case EnemyTypes::hotDog:
			real::Logger::LogInfo("Spawner => Hotdog should spawn");

			SpawnHotDog();
			break;
		case EnemyTypes::egg:
			real::Logger::LogInfo("Spawner => Egg should spawn");

			GetOwner()->CreateGameObject();
			break;
		case EnemyTypes::pickle:
			real::Logger::LogInfo("Spawner => Pickle should spawn");

			GetOwner()->CreateGameObject();
			break;
		}

		m_NeedNewEnemy = false;
	}
}

void Spawner::SpawnEnemy()
{
	if (m_NeedNewEnemy == false)
		m_NeedNewEnemy = true;
	else
		real::Logger::LogWarning("Spawner => A new enemy is already requested.");
}

void Spawner::ReSpawnEnemy(real::GameObject* pEnemy) const
{
	const auto spawnPos = GetOwner()->GetComponent<real::TransformComponent>()->GetWorldPosition();
	pEnemy->GetComponent<real::TransformComponent>()->SetWorldPosition(spawnPos.x, spawnPos.y - 47);
	//pEnemy->GetComponent<real::TransformComponent>()->Translate(0, static_cast<float>(-48 + 1));
}

void Spawner::SpawnHotDog() const
{
	const auto hotDog = GetOwner()->CreateGameObject();
	const auto pTexture = real::ResourceManager::GetInstance().LoadTexture("enemies/hotdog.png");

	hotDog->SetTag(Tags::hot_dog);
	hotDog->GetComponent<real::TransformComponent>()->SetWorldPosition(GetOwner()->GetComponent<real::TransformComponent>()->GetWorldPosition());
	hotDog->GetComponent<real::TransformComponent>()->Translate(0, static_cast<float>(-pTexture->GetSize().y + 1));
	hotDog->AddComponent<real::TextureComponent>()->SetTexture(pTexture);
	hotDog->AddComponent<real::ColliderComponent>(pTexture->GetSize())->EnableDebugRendering(true, Colors::white);
	hotDog->AddComponent<BaseEnemy>();

	const auto core = hotDog->CreateGameObject();
	core->AddComponent<real::ColliderComponent>(glm::vec2{24, 24})->EnableDebugRendering(true, Colors::purple);
	core->GetComponent<real::TransformComponent>()->Translate(12, 12);

	hotDog->Start();
}
