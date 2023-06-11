#include "PlayerDisplay.h"

#include <ResourceManager.h>
#include <TextComponent.h>
#include <TextureComponent.h>
#include <TransformComponent.h>

#include "PlayerManager.h"

PlayerDisplay::PlayerDisplay(real::GameObject* pOwner)
	: Component(pOwner)
{
	PlayerManager::GetInstance().playerJoined.AddObserver(this);
}

PlayerDisplay::~PlayerDisplay()
{
	PlayerManager::GetInstance().playerJoined.RemoveObserver(this);
}

void PlayerDisplay::HandleEvent(int player)
{
	const auto pMiddleFont = real::ResourceManager::GetInstance().LoadFont("fonts/8-bit-hud.ttf", 16);
	const auto pFont = real::ResourceManager::GetInstance().LoadFont("fonts/8-bit-hud.ttf", 10);

	using alignment = real::TextComponent::HorizontalAlignment;

	const auto playerText = GetOwner()->CreateGameObject();
	playerText->AddComponent<real::TextureComponent>();
	playerText->AddComponent<real::TextComponent>()->SetFont(pMiddleFont);
	playerText->GetComponent<real::TextComponent>()->SetText("Player " + std::to_string(player));
	playerText->GetComponent<real::TextComponent>()->ChangeHorizontalAlignment(alignment::center);

	const auto nameText = playerText->CreateGameObject();
	nameText->AddComponent<real::TextureComponent>();
	nameText->AddComponent<real::TextComponent>()->SetFont(pFont);
	nameText->GetComponent<real::TextComponent>()->ChangeHorizontalAlignment(alignment::center);

	switch (player)
	{
	case 1:
	{
		playerText->GetComponent<real::TransformComponent>()->SetLocalPosition(180, 50);
		nameText->GetComponent<real::TransformComponent>()->SetLocalPosition(0, 40);
		nameText->GetComponent<real::TextComponent>()->SetText("Peter Pepper");
		break;
	}
	case 2:
	{
		GetOwner()->GetChildAt(0)->GetComponent<real::TransformComponent>()->SetLocalPosition(0, 50);
		GetOwner()->GetChildAt(0)->GetComponent<real::TextComponent>()->ChangeHorizontalAlignment(alignment::center);
		GetOwner()->GetChildAt(0)->GetChildAt(0)->GetComponent<real::TransformComponent>()->SetLocalPosition(0, 40);
		GetOwner()->GetChildAt(0)->GetChildAt(0)->GetComponent<real::TextComponent>()->SetText("Peter Pepper");

		playerText->GetComponent<real::TransformComponent>()->SetLocalPosition(310, 50);
		nameText->GetComponent<real::TransformComponent>()->SetLocalPosition(15, 40);
		nameText->GetComponent<real::TextComponent>()->SetText("Sally Salt");
		break;
	}
	default: ;
	}
}