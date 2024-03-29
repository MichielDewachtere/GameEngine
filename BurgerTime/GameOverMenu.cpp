﻿#include "GameOverMenu.h"

#include <Input.h>
#include <TextComponent.h>
#include <TextureComponent.h>
#include <TransformComponent.h>
#include <Locator.h>

#include "GameInfo.h"
#include "HighScoreParser.h"
#include "PlayerManager.h"

GameOverMenu::GameOverMenu(real::WindowSettings settings)
	: Scene(Scenes::game_over_menu, InputMaps::menu)
	, m_Settings(settings)
{
}

void GameOverMenu::Load()
{
	using horizontal_alignment = real::TextComponent::HorizontalAlignment;
	using vertical_alignment = real::TextComponent::VerticalAlignment;

	real::Locator::GetAudioSystem().Play(Sounds::menu_background);

	const auto& playerManager = PlayerManager::GetInstance();
	HighScoreParser::UpdateHighScores(playerManager.GetPlayerName(), playerManager.GetScore());
	const auto highScores = HighScoreParser::GetTopFive();

	const auto pFont = real::Locator::GetResourceSystem().LoadFont("fonts/8-bit-hud.ttf", 10);
	const auto pMiddleFont = real::Locator::GetResourceSystem().LoadFont("fonts/8-bit-hud.ttf", 16);
	const auto pTitleFont = real::Locator::GetResourceSystem().LoadFont("fonts/8-bit-hud.ttf", 32);

	const auto pTitleText = CreateGameObject();
	pTitleText->GetComponent<real::TransformComponent>()->SetLocalPosition(m_Settings.width / 2.f, 50);
	pTitleText->AddComponent<real::TextureComponent>();
	pTitleText->AddComponent<real::TextComponent>()->SetFont(pTitleFont);
	pTitleText->GetComponent<real::TextComponent>()->SetText("GAME OVER");
	pTitleText->GetComponent<real::TextComponent>()->SetColor(Colors::white);
	pTitleText->GetComponent<real::TextComponent>()->ChangeHorizontalAlignment(horizontal_alignment::center);

	{
		const auto pNameText = CreateGameObject();
		pNameText->GetComponent<real::TransformComponent>()->SetLocalPosition(m_Settings.width / 2.f - 100, 150);
		pNameText->AddComponent<real::TextureComponent>();
		pNameText->AddComponent<real::TextComponent>()->SetFont(pMiddleFont);
		pNameText->GetComponent<real::TextComponent>()->SetText("Name");
		pNameText->GetComponent<real::TextComponent>()->SetColor(Colors::red);

		const auto pScoreText = CreateGameObject();
		pScoreText->GetComponent<real::TransformComponent>()->SetLocalPosition(m_Settings.width / 2.f + 120, 150);
		pScoreText->AddComponent<real::TextureComponent>();
		pScoreText->AddComponent<real::TextComponent>()->SetFont(pMiddleFont);
		pScoreText->GetComponent<real::TextComponent>()->SetText("Score");
		pScoreText->GetComponent<real::TextComponent>()->SetColor(Colors::red);
	}

	const auto quitText = CreateGameObject();
	quitText->GetComponent<real::TransformComponent>()->SetLocalPosition(10, static_cast<float>(m_Settings.height) - 30);
	quitText->AddComponent<real::TextureComponent>();
	quitText->AddComponent<real::TextComponent>()->SetFont(pFont);
	quitText->GetComponent<real::TextComponent>()->SetText("Press ESC/BACK to exit");
	quitText->GetComponent<real::TextComponent>()->ChangeHorizontalAlignment(horizontal_alignment::right);
	quitText->GetComponent<real::TextComponent>()->ChangeVerticalAlignment(vertical_alignment::down);

	bool isCurrentPlayerInTopFive = false;
	for (size_t i = 0; i < highScores.size(); ++i)
	{
		const float yPos = 200.f + 40.f * static_cast<float>(i);

		auto name = highScores[i].first;
		const auto score = highScores[i].second;

		if (name.empty())
			name = "???";

		const auto pPlaceText = CreateGameObject();
		pPlaceText->GetComponent<real::TransformComponent>()->SetLocalPosition(m_Settings.width / 4.f, yPos);
		pPlaceText->AddComponent<real::TextureComponent>();
		pPlaceText->AddComponent<real::TextComponent>()->SetFont(pMiddleFont);
		pPlaceText->GetComponent<real::TextComponent>()->SetText(std::to_string(i + 1));
		pPlaceText->GetComponent<real::TextComponent>()->SetColor(Colors::red);

		const auto pNameText = CreateGameObject();
		pNameText->GetComponent<real::TransformComponent>()->SetLocalPosition(m_Settings.width / 2.f - 100, yPos);
		pNameText->AddComponent<real::TextureComponent>();
		pNameText->AddComponent<real::TextComponent>()->SetFont(pMiddleFont);
		pNameText->GetComponent<real::TextComponent>()->SetText(name);
		pNameText->GetComponent<real::TextComponent>()->SetColor(Colors::white);


		const auto pScoreText = CreateGameObject();
		pScoreText->GetComponent<real::TransformComponent>()->SetLocalPosition(m_Settings.width / 2.f + 120, yPos);
		pScoreText->AddComponent<real::TextureComponent>();
		pScoreText->AddComponent<real::TextComponent>()->SetFont(pMiddleFont);
		pScoreText->GetComponent<real::TextComponent>()->SetText(std::to_string(score));
		pScoreText->GetComponent<real::TextComponent>()->SetColor(Colors::white);

		if (name == playerManager.GetPlayerName() && score == playerManager.GetScore())
		{
			isCurrentPlayerInTopFive = true;
			pScoreText->GetComponent<real::TextComponent>()->SetColor(Colors::green);
			pPlaceText->GetComponent<real::TextComponent>()->SetColor(Colors::green);
			pNameText->GetComponent<real::TextComponent>()->SetColor(Colors::green);
		}
	}

	if (isCurrentPlayerInTopFive == false)
	{
		const auto pLines = CreateGameObject();
		pLines->GetComponent<real::TransformComponent>()->SetLocalPosition(m_Settings.width / 4.f, 250.f + 40.f * 4);
		pLines->AddComponent<real::TextureComponent>();
		pLines->AddComponent<real::TextComponent>()->SetFont(pMiddleFont);
		pLines->GetComponent<real::TextComponent>()->SetText("------------------------------");
		pLines->GetComponent<real::TextComponent>()->SetColor(Colors::white);

		auto name = playerManager.GetPlayerName();

		if (name.empty())
			name = "???";

		const auto pNameText = CreateGameObject();
		pNameText->GetComponent<real::TransformComponent>()->SetLocalPosition(m_Settings.width / 2.f - 100, 250.f + 40.f * 5);
		pNameText->AddComponent<real::TextureComponent>();
		pNameText->AddComponent<real::TextComponent>()->SetFont(pMiddleFont);
		pNameText->GetComponent<real::TextComponent>()->SetText(name);
		pNameText->GetComponent<real::TextComponent>()->SetColor(Colors::green);

		const auto pScoreText = CreateGameObject();
		pScoreText->GetComponent<real::TransformComponent>()->SetLocalPosition(m_Settings.width / 2.f + 120, 250.f + 40.f * 5);
		pScoreText->AddComponent<real::TextureComponent>();
		pScoreText->AddComponent<real::TextComponent>()->SetFont(pMiddleFont);
		pScoreText->GetComponent<real::TextComponent>()->SetText(std::to_string(playerManager.GetScore()));
		pScoreText->GetComponent<real::TextComponent>()->SetColor(Colors::green);
	}
}
