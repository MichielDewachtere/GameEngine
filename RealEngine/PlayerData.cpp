#include "PlayerData.h"

#include "HealthComponent.h"
#include "TextComponent.h"

void real::PlayerData::Notify(const GameEvent event, GameObject* actor)
{
	if (event == GameEvent::actorDied)
	{
		const int lives = actor->GetParent()->GetComponent<HealthComponent>()->GetLives();

		const auto pTextComponent = actor->GetComponent<TextComponent>();

		if (pTextComponent == nullptr)
			return;

		pTextComponent->SetText("Lives: " + std::to_string(lives));
	}
}