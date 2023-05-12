#include "stdafx.h"
#include "Spawner.h"

#include <GameTime.h>
#include <Logger.h>

#include "TransformComponent.h"

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

			GetOwner()->CreateGameObject();
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
