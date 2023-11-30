#include "LevelDisplay.h"

#include <TransformComponent.h>
#include <SDLResourceManager.h>

#include "PlayerManager.h"

LevelDisplay::LevelDisplay(real::GameObject* pOwner)
	: Component(pOwner)
{
	PlayerManager::GetInstance().levelHasEnded.AddObserver(this);
	real::SceneManager::GetInstance().onSceneSwitch.AddObserver(this);

	const auto pTexture = real::SDLResourceManager::GetInstance().LoadTexture("misc/iconsspritesheet.png");

	m_SpriteSheet.pTexture = pTexture;
	m_SpriteSheet.columns = 6;
	m_SpriteSheet.rows = 1;
	m_SpriteSheet.animTimer = 0;

	InitIcons();
}

LevelDisplay::~LevelDisplay()
{
	PlayerManager::GetInstance().levelHasEnded.RemoveObserver(this);
	real::SceneManager::GetInstance().onSceneSwitch.RemoveObserver(this);
}

void LevelDisplay::HandleEvent()
{
	++m_LevelsCompleted;
}

void LevelDisplay::HandleEvent(real::Scene& scene, real::SceneManager::SceneSwitchState state)
{
	if (state != real::SceneManager::SceneSwitchState::loaded)
		return;

	if (scene.GetName().find("Level") == std::string::npos)
		return;

	int levelsLeft = m_LevelsCompleted;
	const int tenIcons = m_LevelsCompleted / 10;
	levelsLeft = levelsLeft % 10;
	const int fiveIcons = levelsLeft / 5;
	levelsLeft = levelsLeft % 5;
	const int icons = levelsLeft;
	//const int totalIcons = tenIcons + fiveIcons + icons;


	if (tenIcons > 0 && m_TenIcons->GetChildren().empty() == false)
	{
		if (tenIcons > static_cast<int>(m_TenIcons->GetChildCount()))
		{
			const auto pIcon = m_TenIcons->CreateGameObject();
			pIcon->AddComponent<real::SpriteComponent>(m_SpriteSheet)->SelectSprite(4);
			const auto spriteSize = pIcon->GetComponent<real::SpriteComponent>()->GetSpriteSize();
			pIcon->GetComponent<real::TransformComponent>()->Translate(0, -(spriteSize.y + 3) * static_cast<float>(tenIcons - 1));
		}
	}
	else if (tenIcons > 0 && m_TenIcons->GetChildren().empty())
	{
		const auto pIcon = m_TenIcons->CreateGameObject();
		pIcon->AddComponent<real::SpriteComponent>(m_SpriteSheet)->SelectSprite(4);
	}

	if (fiveIcons == 1)
	{
		const auto spriteSize = m_FiveIcon->GetComponent<real::SpriteComponent>()->GetSpriteSize();
		m_FiveIcon->GetComponent<real::SpriteComponent>()->SetIsActive(true);
		m_FiveIcon->GetComponent<real::TransformComponent>()->SetLocalPosition(0, -(spriteSize.y + 3) * static_cast<float>(tenIcons));
	}
	else
		m_FiveIcon->GetComponent<real::SpriteComponent>()->SetIsActive(false);

	if (icons > 0)
	{
		for (int i = 0; i < static_cast<int>(m_NormalIcons->GetChildCount()); ++i)
		{
			const auto pIcon = m_NormalIcons->GetChildAt(i);

			if (i < icons)
			{
				const auto spriteSize = pIcon->GetComponent<real::SpriteComponent>()->GetSpriteSize();
				pIcon->GetComponent<real::SpriteComponent>()->SetIsActive(true);
				pIcon->GetComponent<real::TransformComponent>()->SetLocalPosition(0, -(spriteSize.y + 3) * static_cast<float>(tenIcons + fiveIcons + i));
			}
			else
				pIcon->GetComponent<real::SpriteComponent>()->SetIsActive(false);
		}
	}
}

void LevelDisplay::InitIcons()
{
	m_NormalIcons = GetOwner()->CreateGameObject();
	constexpr int maxIcons = 4;

	for (int i = 0; i < maxIcons; ++i)
	{
		const auto pIcon = m_NormalIcons->CreateGameObject();
		pIcon->AddComponent<real::SpriteComponent>(m_SpriteSheet)->SelectSprite(2);
		const auto spriteSize = pIcon->GetComponent<real::SpriteComponent>()->GetSpriteSize();
		pIcon->GetComponent<real::TransformComponent>()->Translate(0, -(spriteSize.y + 3) * static_cast<float>(i));
		pIcon->GetComponent<real::SpriteComponent>()->SetIsActive(false);
	}

	m_FiveIcon = GetOwner()->CreateGameObject();
	m_FiveIcon->AddComponent<real::SpriteComponent>(m_SpriteSheet)->SelectSprite(3);
	m_FiveIcon->GetComponent<real::SpriteComponent>()->SetIsActive(false);

	m_TenIcons = GetOwner()->CreateGameObject();
}
