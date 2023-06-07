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
#include "SpriteComponent.h"

void Spawner::Update()
{
	if (m_NeedEnemies <= 0)
		return;

	const float dt = real::Time::GetInstance().GetElapsed();
	constexpr float spawnTime = 2;

	m_AccuTime += dt;

	if (m_AccuTime > spawnTime)
	{
		switch (m_Types[m_NeedEnemies - 1])
		{
		case EnemyTypes::hotDog:
			real::Logger::LogInfo("Spawner => Hotdog should spawn");

			SpawnEnemyType(Tags::hot_dog, 100);
			break;
		case EnemyTypes::egg:
			real::Logger::LogInfo("Spawner => Egg should spawn");

			SpawnEnemyType(Tags::egg, 300);
			break;
		case EnemyTypes::pickle:
			real::Logger::LogInfo("Spawner => Pickle should spawn");

			SpawnEnemyType(Tags::pickle, 100);
			break;
		}

		m_AccuTime = 0;

		--m_NeedEnemies;
	}
}

void Spawner::SpawnEnemy(const std::string& type)
{
	//if (m_NeedNewEnemy == false)
	//	m_NeedNewEnemy = true;
	//else
	//	real::Logger::LogWarning("Spawner => A new enemy is already requested.");

	if (type == Tags::hot_dog)
		m_Types.push_back(EnemyTypes::hotDog);
	else if (type == Tags::egg)
		m_Types.push_back(EnemyTypes::egg);
	else if (type == Tags::pickle)
		m_Types.push_back(EnemyTypes::pickle);

	++m_NeedEnemies;
}

void Spawner::ReSpawnEnemy(real::GameObject* pEnemy) const
{
	const auto spawnPos = GetOwner()->GetComponent<real::TransformComponent>()->GetWorldPosition();
	pEnemy->GetComponent<real::TransformComponent>()->SetWorldPosition(spawnPos.x, spawnPos.y - 47);
	//pEnemy->GetComponent<real::TransformComponent>()->Translate(0, static_cast<float>(-48 + 1));
}

void Spawner::SpawnEnemyType(const std::string& type, int /*points*/)
{
	const auto enemy = GetOwner()->CreateGameObject();
	const auto pTexture = real::ResourceManager::GetInstance().LoadTexture("enemies/" + type + "spritesheet.png");

	real::SpriteSheet spriteSheet;
	spriteSheet.pTexture = pTexture;
	spriteSheet.rows = 2;
	spriteSheet.columns = 6;
	spriteSheet.animTimer = 1 / 10.f;

	enemy->SetTag(type);
	enemy->GetComponent<real::TransformComponent>()->SetWorldPosition(GetOwner()->GetComponent<real::TransformComponent>()->GetWorldPosition());
	enemy->AddComponent<real::SpriteComponent>(spriteSheet);
	const auto spriteSize = enemy->GetComponent<real::SpriteComponent>()->GetSpriteSize();
	enemy->AddComponent<real::ColliderComponent>(spriteSize)->EnableDebugRendering(false, Colors::white);
	enemy->AddComponent<BaseEnemy>();
	enemy->GetComponent<real::TransformComponent>()->Translate(0, -spriteSize.y + 1);

	const auto core = enemy->CreateGameObject();
	core->GetComponent<real::TransformComponent>()->SetLocalPosition(spriteSize.x / 4, spriteSize.y / 4);
	core->AddComponent<real::ColliderComponent>(glm::vec2{spriteSize.x / 2, spriteSize.y / 2})->EnableDebugRendering(false, Colors::purple);

	enemy->Start();
}