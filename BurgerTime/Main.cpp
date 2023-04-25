#include "stdafx.h"

#include <Minigin.h> 

#include <GameObject.h>

#include <InputMap.h>

#include <SceneManager.h>
#include <Scene.h>

#include <ResourceManager.h>

#ifdef USE_STEAM
#include <Achievements.h>
#endif 

#include "FPSCounterComponent.h"
#include "HealthComponent.h"
#include "LivesDisplay.h"

#include <TextComponent.h>
#include <TextureComponent.h>
#include <TransformComponent.h>

#include "DamageCommand.h"
#include "LoadNextSceneCommand.h"
#include "MoveCommand.h"

void loadDemoScene();
void loadControlsDisplayScene();

void load()
{
	loadDemoScene();
	loadControlsDisplayScene();

	real::SceneManager::GetInstance().SetSceneActive("Scene2");
	real::Input::GetInstance().SetInputMapActive("menu");

#ifdef USE_STEAM
	auto& achievement = real::Achievements::GetInstance();
	achievement.steamAchievements = new real::SteamAchievements(achievement.achievements.data(), 4);
#endif // USE_STEAM
}

int main(int, char* [])
{
#ifdef USE_STEAM
	if (!SteamAPI_Init())
	{
		std::cerr << "Fatal Error - Steam must be running to play this game (SteamAPI_Init() failed)." << '\n';
		return 1;
	}
	else
		std::cout << "Successfully initialized steam." << '\n';
#endif // USE _STEAM

	real::Minigin engine("../Data/");
	engine.Run(load);

#ifdef USE_STEAM
	SteamAPI_Shutdown();
#endif // USE_STEAM
	return 0;
};

void loadDemoScene()
{
	auto& scene = real::SceneManager::GetInstance().CreateScene("Demo");
	auto& input = real::Input::GetInstance();
	const auto pInputMap = input.AddInputMap("gameplay");
	scene.SetDefaultInputMap(pInputMap->GetName());

	const auto pBackgroundTexture = real::ResourceManager::GetInstance().LoadTexture("background.tga");
	const auto pFontTitle = real::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	const auto pFontFpsCounter = real::ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);
	const auto pFontLiveCounter = real::ResourceManager::GetInstance().LoadFont("8-bit-hud.ttf", 10);
	const auto pLogoTexture = real::ResourceManager::GetInstance().LoadTexture("logo.tga");
	const auto pCharacterTexture = real::ResourceManager::GetInstance().LoadTexture("Character.png");
	const auto pSecondaryCharacterTexture = real::ResourceManager::GetInstance().LoadTexture("SecondaryCharacter.png");

	const auto pBackground = scene.CreateGameObject();
	pBackground->GetComponent<real::TransformComponent>()->SetLocalPosition(0, 0);
	pBackground->AddComponent<real::TextureComponent>()->SetTexture(pBackgroundTexture);

	const auto pTitle = scene.CreateGameObject();
	pTitle->GetComponent<real::TransformComponent>()->SetLocalPosition(80, 60);
	pTitle->AddComponent<real::TextureComponent>();
	pTitle->AddComponent<real::TextComponent>()->SetFont(pFontTitle);
	pTitle->GetComponent<real::TextComponent>()->SetText("Programming 4 Assignment");

	const auto pLogo = scene.CreateGameObject();
	pLogo->GetComponent<real::TransformComponent>()->SetLocalPosition(216, 180);
	pLogo->AddComponent<real::TextureComponent>()->SetTexture(pLogoTexture);

	const auto pFpsCounter = scene.CreateGameObject();
	pFpsCounter->GetComponent<real::TransformComponent>()->SetLocalPosition(2, 2);
	pFpsCounter->AddComponent<real::TextureComponent>();
	pFpsCounter->AddComponent<real::TextComponent>()->SetFont(pFontFpsCounter);
	pFpsCounter->GetComponent<real::TextComponent>()->SetColor(252, 226, 5);
	pFpsCounter->AddComponent<FPSCounterComponent>();

	const auto pWorldCenter = scene.CreateGameObject();
	pWorldCenter->GetComponent<real::TransformComponent>()->SetLocalPosition({ 640 / 2.f, 300 });

	const auto pCharacter = pWorldCenter->CreateGameObject();
	pCharacter->GetComponent<real::TransformComponent>()->SetLocalPosition({ 20,0 });
	pCharacter->AddComponent<real::TextureComponent>()->SetTexture(pCharacterTexture);
	pCharacter->AddComponent<HealthComponent>()->SetLives(4);
	pCharacter->GetComponent<HealthComponent>()->SetSpawnPoint(pCharacter->GetComponent<real::TransformComponent>()->GetWorldPosition());

	const auto pLiveCounterP1 = pCharacter->CreateGameObject();
	pLiveCounterP1->GetComponent<real::TransformComponent>()->SetLocalPosition(-338, 100);
	pLiveCounterP1->AddComponent<real::TextureComponent>();
	pLiveCounterP1->AddComponent<real::TextComponent>()->SetColor(254, 194, 12);
	pLiveCounterP1->GetComponent<real::TextComponent>()->SetFont(pFontLiveCounter);
	pLiveCounterP1->AddComponent<LivesDisplay>(pCharacter->GetComponent<HealthComponent>());

	const auto pPointCounterP1 = pCharacter->CreateGameObject();
	pPointCounterP1->GetComponent<real::TransformComponent>()->SetLocalPosition(-338, 118);
	pPointCounterP1->AddComponent<real::TextureComponent>();
	pPointCounterP1->AddComponent<real::TextComponent>()->SetColor(254, 194, 12);
	pPointCounterP1->GetComponent<real::TextComponent>()->SetFont(pFontLiveCounter);
	pPointCounterP1->GetComponent<real::TextComponent>()->SetText("Points: 0");

	const auto pSecondaryCharacter = pWorldCenter->CreateGameObject();
	pSecondaryCharacter->GetComponent<real::TransformComponent>()->SetLocalPosition({ 40,0 });
	pSecondaryCharacter->AddComponent<real::TextureComponent>()->SetTexture(pSecondaryCharacterTexture);
	pSecondaryCharacter->AddComponent<HealthComponent>()->SetLives(4);
	pSecondaryCharacter->GetComponent<HealthComponent>()->SetSpawnPoint(pSecondaryCharacter->GetComponent<real::TransformComponent>()->GetWorldPosition());

	const auto pLiveCounterP2 = pSecondaryCharacter->CreateGameObject();
	pLiveCounterP2->GetComponent<real::TransformComponent>()->SetLocalPosition(-358, 140);
	pLiveCounterP2->AddComponent<real::TextureComponent>();
	pLiveCounterP2->AddComponent<real::TextComponent>()->SetColor(50, 205, 50);
	pLiveCounterP2->GetComponent<real::TextComponent>()->SetFont(pFontLiveCounter);
	pLiveCounterP2->AddComponent<LivesDisplay>(pSecondaryCharacter->GetComponent<HealthComponent>());

	const auto pPointCounterP2 = pSecondaryCharacter->CreateGameObject();
	pPointCounterP2->GetComponent<real::TransformComponent>()->SetLocalPosition(-358, 158);
	pPointCounterP2->AddComponent<real::TextureComponent>();
	pPointCounterP2->AddComponent<real::TextComponent>()->SetColor(50, 205, 50);
	pPointCounterP2->GetComponent<real::TextComponent>()->SetFont(pFontLiveCounter);
	pPointCounterP2->GetComponent<real::TextComponent>()->SetText("Points: 0");
	//pPointCounterP2->AddObserver<real::PlayerData>();

	input.EnableCoOp(true);
	const auto controllerIdcs = input.AddControllers();

	if (controllerIdcs.empty())
	{
		// only keyboard
		input.UseKeyboard(true);
		pInputMap->AddKeyboardCommands<real::MoveCommand>(SDL_SCANCODE_A, KEYPRESSED, pCharacter);
		pInputMap->AddKeyboardCommands<real::MoveCommand>(SDL_SCANCODE_D, KEYPRESSED, pCharacter);
		pInputMap->AddKeyboardCommands<real::MoveCommand>(SDL_SCANCODE_S, KEYPRESSED, pCharacter);
		pInputMap->AddKeyboardCommands<real::MoveCommand>(SDL_SCANCODE_W, KEYPRESSED, pCharacter);
		pInputMap->AddKeyboardCommands<DamageCommand>(SDL_SCANCODE_SPACE, SDL_KEYUP, pCharacter);
		//pInputMap->AddKeyboardCommands<real::AddPointsCommand>(SDL_SCANCODE_LSHIFT, SDL_KEYUP, pCharacter);
	}
	else if (!controllerIdcs.empty())
	{
		using ControllerButton = real::XInputController::ControllerButton;
		using InputType = real::XInputController::InputType;

		// 1 controller (and 1 keyboard if co-op)
		pInputMap->AddControllerCommands<real::MoveCommand>(ControllerButton::LeftThumb, InputType::leftThumbMoved, controllerIdcs[0], pCharacter);
		pInputMap->AddControllerCommands<real::TestCommand>(ControllerButton::ButtonDown, InputType::down, controllerIdcs[0], pCharacter);
		pInputMap->AddControllerCommands<DamageCommand>(ControllerButton::ButtonLeft, InputType::down, controllerIdcs[0], pCharacter);
		//pInputMap->AddControllerCommands<real::AddPointsCommand>(ControllerButton::ButtonUp, InputType::down, controllerIdcs[0], pCharacter);

		if (input.IsCoOpEnabled() == true)
		{
			if (controllerIdcs.size() == 2)
			{
				input.UseKeyboard(false);

				pInputMap->AddControllerCommands<real::MoveCommand>(ControllerButton::RightThumb, InputType::leftThumbMoved, controllerIdcs[1], pSecondaryCharacter, 100.f);
				pInputMap->AddControllerCommands<DamageCommand>(ControllerButton::ButtonLeft, InputType::down, controllerIdcs[1], pSecondaryCharacter);
				//pInputMap->AddControllerCommands<real::AddPointsCommand>(ControllerButton::ButtonUp, InputType::down, controllerIdcs[1], pSecondaryCharacter);
			}
			else
			{
				input.UseKeyboard(true);
				pInputMap->AddKeyboardCommands<real::MoveCommand>(SDL_SCANCODE_A, KEYPRESSED, pSecondaryCharacter, 100.f);
				pInputMap->AddKeyboardCommands<real::MoveCommand>(SDL_SCANCODE_D, KEYPRESSED, pSecondaryCharacter, 100.f);
				pInputMap->AddKeyboardCommands<real::MoveCommand>(SDL_SCANCODE_S, KEYPRESSED, pSecondaryCharacter, 100.f);
				pInputMap->AddKeyboardCommands<real::MoveCommand>(SDL_SCANCODE_W, KEYPRESSED, pSecondaryCharacter, 100.f);
				pInputMap->AddKeyboardCommands<DamageCommand>(SDL_SCANCODE_SPACE, SDL_KEYUP, pSecondaryCharacter);
				//pInputMap->AddKeyboardCommands<real::AddPointsCommand>(SDL_SCANCODE_LSHIFT, SDL_KEYUP, pSecondaryCharacter);
			}
		}
		else
			input.UseKeyboard(false);
	}
}
void loadControlsDisplayScene()
{
	auto& scene = real::SceneManager::GetInstance().CreateScene("Scene2");
	auto& input = real::Input::GetInstance();
	const auto pInputMap = input.AddInputMap("menu");

	const auto pBackgroundTexture = real::ResourceManager::GetInstance().LoadTexture("background.tga");
	const auto pFont = real::ResourceManager::GetInstance().LoadFont("8-bit-hud.ttf", 10);

	const auto pBackground2 = scene.CreateGameObject();
	pBackground2->GetComponent<real::TransformComponent>()->SetLocalPosition(0, 0);
	pBackground2->AddComponent<real::TextureComponent>()->SetTexture(pBackgroundTexture);

	const auto pTitleText = scene.CreateGameObject();
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
	pText4->GetComponent<real::TextComponent>()->SetText("Move                 Left Joystick");

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
	const auto pContinueText = scene.CreateGameObject();
	pContinueText->GetComponent<real::TransformComponent>()->SetLocalPosition(350, 450);
	pContinueText->AddComponent<real::TextureComponent>();
	pContinueText->AddComponent<real::TextComponent>()->SetFont(pFont);
	pContinueText->GetComponent<real::TextComponent>()->SetText("To continue, press Space");

	pInputMap->AddKeyboardCommands<LoadNextSceneCommand>(SDL_SCANCODE_SPACE, SDL_KEYUP, nullptr, "Demo");
	pInputMap->AddControllerCommands<LoadNextSceneCommand>(real::XInputController::ControllerButton::ButtonDown, real::XInputController::InputType::down, (unsigned int)-1, nullptr, "Demo");
}