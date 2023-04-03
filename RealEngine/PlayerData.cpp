#include "PlayerData.h"

#include "HealthComponent.h"
#include "TextComponent.h"

#include "Achievements.h"

void real::PlayerData::Notify(const GameEvent event, GameObject* actor)
{
	switch (event)
	{
	case GameEvent::actorDied:
	{
		const int lives = actor->GetParent()->GetComponent<HealthComponent>()->GetLives();

		const auto pTextComponent = actor->GetComponent<TextComponent>();

		if (pTextComponent == nullptr)
			return;

		const std::string keyword = "Lives";

		if (pTextComponent->GetText().find(keyword) == std::string::npos)
			return;

		pTextComponent->SetText("Lives: " + std::to_string(lives));
		break;
	}
	case GameEvent::actorGainedPoints:
	{
		//This is for testing purposes
		//TODO: finished version calls AttackComponent or something, this is still tbd.
		m_AmountOfPoints += 50;

		const auto& achievements = Achievements::GetInstance();

		if (m_AmountOfPoints >= 500 && achievements.steamAchievements)
			achievements.steamAchievements->SetAchievement("ACH_WIN_ONE_GAME");

		const auto pTextComponent = actor->GetComponent<TextComponent>();

		if (pTextComponent == nullptr)
			return;

		const std::string keyword = "Points";

		if (pTextComponent->GetText().find(keyword) == std::string::npos)
			return;


		pTextComponent->SetText("Points: " + std::to_string(m_AmountOfPoints));
		break;
	}
	}
}