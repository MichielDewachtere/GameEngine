#include "MainMenu.h"

#include <Input.h>
#include <InputMap.h>
#include <ResourceManager.h>
#include <TextComponent.h>
#include <TextureComponent.h>
#include <TransformComponent.h>

#include "LoadNextSceneCommand.h"
#include "GameInfo.h"
#include "PlayerJoinCommand.h"
#include "PlayerManager.h"

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

	using alignment = real::TextComponent::Alignment;

	const auto pFont = real::ResourceManager::GetInstance().LoadFont("fonts/8-bit-hud.ttf", 10);
	const auto pMiddleFont = real::ResourceManager::GetInstance().LoadFont("fonts/8-bit-hud.ttf", 16);
	const auto pTitleFont = real::ResourceManager::GetInstance().LoadFont("fonts/8-bit-hud.ttf", 32);

	const auto pTitleText = CreateGameObject();
	pTitleText->GetComponent<real::TransformComponent>()->SetLocalPosition(m_Settings.width / 2.f, 30);
	pTitleText->AddComponent<real::TextureComponent>();
	pTitleText->AddComponent<real::TextComponent>()->SetFont(pTitleFont);
	pTitleText->GetComponent<real::TextComponent>()->SetText("Burger Time");
	pTitleText->GetComponent<real::TextComponent>()->SetColor(Colors::red);
	pTitleText->GetComponent<real::TextComponent>()->ChangeAlignment(alignment::center);

	//const auto pKeyBoardText = pTitleText->CreateGameObject<real::TextObject>();
	//pKeyBoardText->InitComponents({ 0,20 }, pFont, "Keyboard: ");

	const auto pControlsText = CreateGameObject();
	pControlsText->GetComponent<real::TransformComponent>()->SetLocalPosition(m_Settings.width / 2.f, 100);
	pControlsText->AddComponent<real::TextureComponent>();
	pControlsText->AddComponent<real::TextComponent>()->SetFont(pMiddleFont);
	pControlsText->GetComponent<real::TextComponent>()->SetText("Controls");
	pControlsText->GetComponent<real::TextComponent>()->SetColor(Colors::white);
	pControlsText->GetComponent<real::TextComponent>()->ChangeAlignment(alignment::center);

	const auto pKeyBoardText = pControlsText->CreateGameObject();
	pKeyBoardText->GetComponent<real::TransformComponent>()->SetLocalPosition(-m_Settings.width / 6.f, 40);
	pKeyBoardText->AddComponent<real::TextureComponent>();
	pKeyBoardText->AddComponent<real::TextComponent>()->SetFont(pFont);
	pKeyBoardText->GetComponent<real::TextComponent>()->SetText("Keyboard:");

	const auto pText1 = pControlsText->CreateGameObject();
	pText1->GetComponent<real::TransformComponent>()->SetLocalPosition(-m_Settings.width / 6.f, 60);
	pText1->AddComponent<real::TextureComponent>();
	pText1->AddComponent<real::TextComponent>()->SetFont(pFont);
	pText1->GetComponent<real::TextComponent>()->SetText("Move                 UP, LEFT, DOWN, RIGHT");

	const auto pText2 = pControlsText->CreateGameObject();
	pText2->GetComponent<real::TransformComponent>()->SetLocalPosition(-m_Settings.width / 6.f, 80);
	pText2->AddComponent<real::TextureComponent>();
	pText2->AddComponent<real::TextComponent>()->SetFont(pFont);
	pText2->GetComponent<real::TextComponent>()->SetText("Use pepper      Z/X");

	const auto pText3 = pControlsText->CreateGameObject();
	pText3->GetComponent<real::TransformComponent>()->SetLocalPosition(-m_Settings.width / 6.f, 100);
	pText3->AddComponent<real::TextureComponent>();
	pText3->AddComponent<real::TextComponent>()->SetFont(pFont);
	pText3->GetComponent<real::TextComponent>()->SetText("Mute                 M");


	const auto pControllerText = pControlsText->CreateGameObject();
	pControllerText->GetComponent<real::TransformComponent>()->SetLocalPosition(-m_Settings.width / 6.f, 140);
	pControllerText->AddComponent<real::TextureComponent>();
	pControllerText->AddComponent<real::TextComponent>()->SetFont(pFont);
	pControllerText->GetComponent<real::TextComponent>()->SetText("Controller:");

	const auto pText4 = pControlsText->CreateGameObject();
	pText4->GetComponent<real::TransformComponent>()->SetLocalPosition(-m_Settings.width / 6.f, 160);
	pText4->AddComponent<real::TextureComponent>();
	pText4->AddComponent<real::TextComponent>()->SetFont(pFont);
	pText4->GetComponent<real::TextComponent>()->SetText("Move                 DPAD");

	const auto pText5 = pControlsText->CreateGameObject();
	pText5->GetComponent<real::TransformComponent>()->SetLocalPosition(-m_Settings.width / 6.f, 180);
	pText5->AddComponent<real::TextureComponent>();
	pText5->AddComponent<real::TextComponent>()->SetFont(pFont);
	pText5->GetComponent<real::TextComponent>()->SetText("Use pepper      A/X");

	const auto pText6 = pControlsText->CreateGameObject();
	pText6->GetComponent<real::TransformComponent>()->SetLocalPosition(-m_Settings.width / 6.f, 200);
	pText6->AddComponent<real::TextureComponent>();
	pText6->AddComponent<real::TextComponent>()->SetFont(pFont);
	pText6->GetComponent<real::TextComponent>()->SetText("Mute                 Y");
	
	const auto pJoinText = CreateGameObject();
	pJoinText->GetComponent<real::TransformComponent>()->SetLocalPosition(m_Settings.width / 2.f, 350);
	pJoinText->AddComponent<real::TextureComponent>();
	pJoinText->AddComponent<real::TextComponent>()->SetFont(pMiddleFont);
	pJoinText->GetComponent<real::TextComponent>()->SetText("Press ENTER/A to join");
	pJoinText->GetComponent<real::TextComponent>()->ChangeAlignment(alignment::center);

	//const auto pContinueText = this->CreateGameObject();
	//pContinueText->GetComponent<real::TransformComponent>()->SetLocalPosition(320, 450);
	//pContinueText->AddComponent<real::TextureComponent>();
	//pContinueText->AddComponent<real::TextComponent>()->SetFont(pFont);
	//pContinueText->GetComponent<real::TextComponent>()->SetText("To continue, press Space");

	input.UseKeyboard(true);
	input.EnableCoOp(true);
	const auto controllerIdcs = input.AddControllers();

	pInputMap->AddKeyboardCommands<PlayerJoinCommand>(SDL_SCANCODE_RETURN, SDL_KEYUP, nullptr);
	pInputMap->AddKeyboardCommands<LoadNextSceneCommand>(SDL_SCANCODE_SPACE, SDL_KEYUP, nullptr, Scenes::level01);

	if (controllerIdcs.empty() == false)
	{
		pInputMap->AddControllerCommands<PlayerJoinCommand>(real::XInputController::ControllerButton::ButtonDown, real::XInputController::InputType::down, -1, nullptr);
		pInputMap->AddControllerCommands<LoadNextSceneCommand>(real::XInputController::ControllerButton::Start, real::XInputController::InputType::down, 0, nullptr, Scenes::level01);
	}

	m_IsLoaded = true;

	//PlayerManager::GetInstance().AddPlayer();
}