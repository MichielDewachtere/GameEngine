#include "MainMenu.h"

#include <Input.h>
#include <InputMap.h>
#include <ResourceManager.h>
#include <TextComponent.h>
#include <TextureComponent.h>
#include <TransformComponent.h>
#include <Locator.h>

#include "LoadNextLevelCommand.h"
#include "GameInfo.h"
#include "PlayerDisplay.h"
#include "PlayerJoinCommand.h"
#include "PlayerManager.h"
#include "PlayerNameDisplay.h"
#include "QuitCommand.h"


MainMenu::MainMenu(real::WindowSettings settings)
	: Scene(Scenes::main_menu, InputMaps::menu)
	, m_Settings(std::move(settings))
{
}

void MainMenu::Load()
{
	//auto& scene = real::SceneManager::GetInstance().AddScene(this);
	auto& input = real::Input::GetInstance();
	const auto pInputMap = input.AddInputMap(InputMaps::menu);

	real::Locator::GetAudioSystem().Play(Sounds::menu_background);

	using vertical_alignment = real::TextComponent::HorizontalAlignment;
	using horizontal_alignment = real::TextComponent::VerticalAlignment;

	const auto pFont = real::ResourceManager::GetInstance().LoadFont("fonts/8-bit-hud.ttf", 10);
	const auto pMiddleFont = real::ResourceManager::GetInstance().LoadFont("fonts/8-bit-hud.ttf", 16);
	const auto pTitleFont = real::ResourceManager::GetInstance().LoadFont("fonts/8-bit-hud.ttf", 32);

	const auto pTitleText = CreateGameObject();
	pTitleText->GetComponent<real::TransformComponent>()->SetLocalPosition(static_cast<float>(m_Settings.width) / 2.f, 30);
	pTitleText->AddComponent<real::TextureComponent>();
	pTitleText->AddComponent<real::TextComponent>()->SetFont(pTitleFont);
	pTitleText->GetComponent<real::TextComponent>()->SetText("Burger Time");
	pTitleText->GetComponent<real::TextComponent>()->SetColor(Colors::red);
	pTitleText->GetComponent<real::TextComponent>()->ChangeHorizontalAlignment(vertical_alignment::center);

	//const auto pKeyBoardText = pTitleText->CreateGameObject<real::TextObject>();
	//pKeyBoardText->InitComponents({ 0,20 }, pFont, "Keyboard: ");

	const auto pControlsText = CreateGameObject();
	pControlsText->GetComponent<real::TransformComponent>()->SetLocalPosition(static_cast<float>(m_Settings.width) / 2.f, 100);
	pControlsText->AddComponent<real::TextureComponent>();
	pControlsText->AddComponent<real::TextComponent>()->SetFont(pMiddleFont);
	pControlsText->GetComponent<real::TextComponent>()->SetText("Controls");
	pControlsText->GetComponent<real::TextComponent>()->SetColor(Colors::white);
	pControlsText->GetComponent<real::TextComponent>()->ChangeHorizontalAlignment(vertical_alignment::center);

	const auto pKeyBoardText = pControlsText->CreateGameObject();
	pKeyBoardText->GetComponent<real::TransformComponent>()->SetLocalPosition(static_cast<float>(-m_Settings.width) / 6.f, 40);
	pKeyBoardText->AddComponent<real::TextureComponent>();
	pKeyBoardText->AddComponent<real::TextComponent>()->SetFont(pFont);
	pKeyBoardText->GetComponent<real::TextComponent>()->SetText("Keyboard:");

	const auto pText1 = pControlsText->CreateGameObject();
	pText1->GetComponent<real::TransformComponent>()->SetLocalPosition(static_cast<float>(-m_Settings.width) / 6.f, 60);
	pText1->AddComponent<real::TextureComponent>();
	pText1->AddComponent<real::TextComponent>()->SetFont(pFont);
	pText1->GetComponent<real::TextComponent>()->SetText("Move                 UP, LEFT, DOWN, RIGHT");

	const auto pText2 = pControlsText->CreateGameObject();
	pText2->GetComponent<real::TransformComponent>()->SetLocalPosition(static_cast<float>(-m_Settings.width) / 6.f, 80);
	pText2->AddComponent<real::TextureComponent>();
	pText2->AddComponent<real::TextComponent>()->SetFont(pFont);
	pText2->GetComponent<real::TextComponent>()->SetText("Use pepper      Z/X");

	const auto pText3 = pControlsText->CreateGameObject();
	pText3->GetComponent<real::TransformComponent>()->SetLocalPosition(static_cast<float>(-m_Settings.width) / 6.f, 100);
	pText3->AddComponent<real::TextureComponent>();
	pText3->AddComponent<real::TextComponent>()->SetFont(pFont);
	pText3->GetComponent<real::TextComponent>()->SetText("Mute                 M");


	const auto pControllerText = pControlsText->CreateGameObject();
	pControllerText->GetComponent<real::TransformComponent>()->SetLocalPosition(static_cast<float>(-m_Settings.width) / 6.f, 140);
	pControllerText->AddComponent<real::TextureComponent>();
	pControllerText->AddComponent<real::TextComponent>()->SetFont(pFont);
	pControllerText->GetComponent<real::TextComponent>()->SetText("Controller:");

	const auto pText4 = pControlsText->CreateGameObject();
	pText4->GetComponent<real::TransformComponent>()->SetLocalPosition(static_cast<float>(-m_Settings.width) / 6.f, 160);
	pText4->AddComponent<real::TextureComponent>();
	pText4->AddComponent<real::TextComponent>()->SetFont(pFont);
	pText4->GetComponent<real::TextComponent>()->SetText("Move                 DPAD");

	const auto pText5 = pControlsText->CreateGameObject();
	pText5->GetComponent<real::TransformComponent>()->SetLocalPosition(static_cast<float>(-m_Settings.width) / 6.f, 180);
	pText5->AddComponent<real::TextureComponent>();
	pText5->AddComponent<real::TextComponent>()->SetFont(pFont);
	pText5->GetComponent<real::TextComponent>()->SetText("Use pepper      A/X");

	const auto pText6 = pControlsText->CreateGameObject();
	pText6->GetComponent<real::TransformComponent>()->SetLocalPosition(static_cast<float>(-m_Settings.width) / 6.f, 200);
	pText6->AddComponent<real::TextureComponent>();
	pText6->AddComponent<real::TextComponent>()->SetFont(pFont);
	pText6->GetComponent<real::TextComponent>()->SetText("Mute                 Y");
	
	const auto pJoinText = CreateGameObject();
	pJoinText->GetComponent<real::TransformComponent>()->SetLocalPosition(static_cast<float>(m_Settings.width) / 2.f, 350);
	pJoinText->AddComponent<real::TextureComponent>();
	pJoinText->AddComponent<real::TextComponent>()->SetFont(pMiddleFont);
	pJoinText->GetComponent<real::TextComponent>()->SetText("Press ENTER/A to join");
	pJoinText->GetComponent<real::TextComponent>()->ChangeHorizontalAlignment(vertical_alignment::center);
	pJoinText->AddComponent<PlayerDisplay>();

	const auto playerNameText = CreateGameObject();
	playerNameText->GetComponent<real::TransformComponent>()->SetLocalPosition(static_cast<float>(m_Settings.width) / 2.f, 500);
	playerNameText->AddComponent<real::TextureComponent>();
	playerNameText->AddComponent<real::TextComponent>()->SetFont(pMiddleFont);
	playerNameText->GetComponent<real::TextComponent>()->SetText("Player/Team name");
	playerNameText->GetComponent<real::TextComponent>()->ChangeHorizontalAlignment(vertical_alignment::center);

	const auto playerName = CreateGameObject();
	playerName->GetComponent<real::TransformComponent>()->SetLocalPosition(static_cast<float>(m_Settings.width) / 2.f, 540);
	playerName->AddComponent<real::TextureComponent>();
	playerName->AddComponent<real::TextComponent>()->SetFont(pFont);
	playerName->GetComponent<real::TextComponent>()->SetColor(Colors::green);
	playerName->GetComponent<real::TextComponent>()->SetText(" ");
	playerName->GetComponent<real::TextComponent>()->ChangeHorizontalAlignment(vertical_alignment::center);
	playerName->AddComponent<PlayerNameDisplay>();

	const auto startText = CreateGameObject();
	startText->GetComponent<real::TransformComponent>()->SetLocalPosition(static_cast<float>(m_Settings.width)
		- 10, static_cast<float>(m_Settings.height) - 60);
	startText->AddComponent<real::TextureComponent>();
	startText->AddComponent<real::TextComponent>()->SetFont(pFont);
	startText->GetComponent<real::TextComponent>()->SetText("Press SPACE/START to start");
	startText->GetComponent<real::TextComponent>()->ChangeHorizontalAlignment(vertical_alignment::left);
	startText->GetComponent<real::TextComponent>()->ChangeVerticalAlignment(horizontal_alignment::down);

	const auto quitText = CreateGameObject();
	quitText->GetComponent<real::TransformComponent>()->SetLocalPosition(10, static_cast<float>(m_Settings.height) - 30);
	quitText->AddComponent<real::TextureComponent>();
	quitText->AddComponent<real::TextComponent>()->SetFont(pFont);
	quitText->GetComponent<real::TextComponent>()->SetText("Press ESC/BACK to exit");
	quitText->GetComponent<real::TextComponent>()->ChangeHorizontalAlignment(vertical_alignment::right);
	quitText->GetComponent<real::TextComponent>()->ChangeVerticalAlignment(horizontal_alignment::down);

	input.UseKeyboard(true);
	input.EnableCoOp(true);
	const auto controllerIdcs = input.AddControllers();

	pInputMap->AddKeyboardCommands<PlayerJoinCommand>(SDL_SCANCODE_RETURN, SDL_KEYUP, nullptr);
	pInputMap->AddKeyboardCommands<LoadNextLevelCommand>(SDL_SCANCODE_SPACE, SDL_KEYUP, nullptr, Scenes::level01);
	pInputMap->AddKeyboardCommands<QuitCommand>(SDL_SCANCODE_ESCAPE, SDL_KEYUP, nullptr);

	if (controllerIdcs.empty() == false)
	{
		pInputMap->AddControllerCommands<PlayerJoinCommand>(real::XInputController::ControllerButton::ButtonDown, real::XInputController::InputType::down, -1, nullptr);
		pInputMap->AddControllerCommands<LoadNextLevelCommand>(real::XInputController::ControllerButton::Start, real::XInputController::InputType::down, -1, nullptr, Scenes::level01);
		pInputMap->AddControllerCommands<QuitCommand>(real::XInputController::ControllerButton::Back, real::XInputController::InputType::down, -1, nullptr);
	}

	m_IsLoaded = true;
}