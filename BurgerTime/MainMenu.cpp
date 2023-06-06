#include "MainMenu.h"

#include <Input.h>
#include <InputMap.h>
#include <ResourceManager.h>
#include <TextComponent.h>
#include <TextureComponent.h>
#include <TransformComponent.h>

#include "LoadNextSceneCommand.h"
#include "GameInfo.h"

MainMenu::MainMenu()
	: Scene(Scenes::main_menu, InputMaps::menu)
{
}

void MainMenu::Load()
{
	//auto& scene = real::SceneManager::GetInstance().AddScene(this);
	auto& input = real::Input::GetInstance();
	const auto pInputMap = input.AddInputMap(InputMaps::menu);

	const auto pBackgroundTexture = real::ResourceManager::GetInstance().LoadTexture("background.tga");
	const auto pFont = real::ResourceManager::GetInstance().LoadFont("8-bit-hud.ttf", 10);

	const auto pBackground2 = this->CreateGameObject();
	pBackground2->GetComponent<real::TransformComponent>()->SetLocalPosition(0, 0);
	pBackground2->AddComponent<real::TextureComponent>()->SetTexture(pBackgroundTexture);

	const auto pTitleText = this->CreateGameObject();
	pTitleText->GetComponent<real::TransformComponent>()->SetLocalPosition(50, 30);
	pTitleText->AddComponent<real::TextureComponent>();
	pTitleText->AddComponent<real::TextComponent>()->SetFont(pFont);
	pTitleText->GetComponent<real::TextComponent>()->SetText("Player Controls");
	pTitleText->GetComponent<real::TextComponent>()->SetColor({ 255,0,0,0 });

	//const auto pKeyBoardText = pTitleText->CreateGameObject<real::TextObject>();
	//pKeyBoardText->InitComponents({ 0,20 }, pFont, "Keyboard: ");
	const auto pKeyBoardText = pTitleText->CreateGameObject();
	pKeyBoardText->GetComponent<real::TransformComponent>()->SetLocalPosition(0, 20);
	pKeyBoardText->AddComponent<real::TextureComponent>();
	pKeyBoardText->AddComponent<real::TextComponent>()->SetFont(pFont);
	pKeyBoardText->GetComponent<real::TextComponent>()->SetText("Keyboard: ");

	//const auto pText1 = pTitleText->CreateGameObject<real::TextObject>();
	//pText1->InitComponents({ 0,40 }, pFont, "Move                 W, A, S, D");
	const auto pText1 = pTitleText->CreateGameObject();
	pText1->GetComponent<real::TransformComponent>()->SetLocalPosition(0, 40);
	pText1->AddComponent<real::TextureComponent>();
	pText1->AddComponent<real::TextComponent>()->SetFont(pFont);
	pText1->GetComponent<real::TextComponent>()->SetText("Move                 W, A, S, D");

	//const auto pText2 = pTitleText->CreateGameObject<real::TextObject>();
	//pText2->InitComponents({ 0,60 }, pFont, "Remove Life    Space");
	const auto pText2 = pTitleText->CreateGameObject();
	pText2->GetComponent<real::TransformComponent>()->SetLocalPosition(0, 60);
	pText2->AddComponent<real::TextureComponent>();
	pText2->AddComponent<real::TextComponent>()->SetFont(pFont);
	pText2->GetComponent<real::TextComponent>()->SetText("Remove Life    Space");

	//const auto pText3 = pTitleText->CreateGameObject<real::TextObject>();
	//pText3->InitComponents({ 0,80 }, pFont, "Add Points       LShift");
	const auto pText3 = pTitleText->CreateGameObject();
	pText3->GetComponent<real::TransformComponent>()->SetLocalPosition(0, 80);
	pText3->AddComponent<real::TextureComponent>();
	pText3->AddComponent<real::TextComponent>()->SetFont(pFont);
	pText3->GetComponent<real::TextComponent>()->SetText("Remove Life    Space");

	//const auto pControllerText = pTitleText->CreateGameObject<real::TextObject>();
	//pControllerText->InitComponents({ 0,130 }, pFont, "Controller: ");
	const auto pControllerText = pTitleText->CreateGameObject();
	pControllerText->GetComponent<real::TransformComponent>()->SetLocalPosition(0, 80);
	pControllerText->AddComponent<real::TextureComponent>();
	pControllerText->AddComponent<real::TextComponent>()->SetFont(pFont);
	pControllerText->GetComponent<real::TextComponent>()->SetText("Remove Life    Space");

	//const auto pText4 = pTitleText->CreateGameObject<real::TextObject>();
	//pText4->InitComponents({ 0,150 }, pFont, "Move                 Left Joystick");
	const auto pText4 = pTitleText->CreateGameObject();
	pText4->GetComponent<real::TransformComponent>()->SetLocalPosition(0, 150);
	pText4->AddComponent<real::TextureComponent>();
	pText4->AddComponent<real::TextComponent>()->SetFont(pFont);
	pText4->GetComponent<real::TextComponent>()->SetText("Move                 DPAD");

	//const auto pText5 = pTitleText->CreateGameObject<real::TextObject>();
	//pText5->InitComponents({ 0,170 }, pFont, "Remove Life    Left Button");
	const auto pText5 = pTitleText->CreateGameObject();
	pText5->GetComponent<real::TransformComponent>()->SetLocalPosition(0, 170);
	pText5->AddComponent<real::TextureComponent>();
	pText5->AddComponent<real::TextComponent>()->SetFont(pFont);
	pText5->GetComponent<real::TextComponent>()->SetText("Remove Life    Left Button");

	//const auto pText6 = pTitleText->CreateGameObject<real::TextObject>();
	//pText6->InitComponents({ 0,190 }, pFont, "Add Points      Right Button");
	const auto pText6 = pTitleText->CreateGameObject();
	pText6->GetComponent<real::TransformComponent>()->SetLocalPosition(0, 190);
	pText6->AddComponent<real::TextureComponent>();
	pText6->AddComponent<real::TextComponent>()->SetFont(pFont);
	pText6->GetComponent<real::TextComponent>()->SetText("Add Points      Right Button");

	//const auto pContinueText = scene.CreateGameObject<real::TextObject>();
	//pContinueText->InitComponents({ 350, 450 }, pFont, "To continue, press Space");
	const auto pContinueText = this->CreateGameObject();
	pContinueText->GetComponent<real::TransformComponent>()->SetLocalPosition(320, 450);
	pContinueText->AddComponent<real::TextureComponent>();
	pContinueText->AddComponent<real::TextComponent>()->SetFont(pFont);
	pContinueText->GetComponent<real::TextComponent>()->SetText("To continue, press Space");

	input.UseKeyboard(true);
	pInputMap->AddKeyboardCommands<LoadNextSceneCommand>(SDL_SCANCODE_SPACE, SDL_KEYUP, nullptr, Scenes::level01);
	pInputMap->AddControllerCommands<LoadNextSceneCommand>(real::XInputController::ControllerButton::ButtonDown, real::XInputController::InputType::down, (unsigned int)-1, nullptr, Scenes::level01);

	m_IsLoaded = true;
}