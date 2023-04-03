#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include <steam_api.h>

#include <iostream>

#include "Achievements.h"
#include "GameObject.h"
#include "Input.h"
#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "Command.h"

#include "FPSCounterComponent.h"
#include "RotatorComponent.h"
#include "TextureComponent.h"
#include "TextComponent.h"
#include "TransformComponent.h"
#include "HealthComponent.h"
#include "PlayerData.h"

namespace real
{
	//class PlayerData;
}

void load()
{
	auto& scene = real::SceneManager::GetInstance().CreateScene("Demo");
	auto& input = real::Input::GetInstance();
	auto& achievement = real::Achievements::GetInstance();

	const auto pBackgroundTexture = real::ResourceManager::GetInstance().LoadTexture("background.tga");
	const auto pFontTitle = real::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	const auto pFontFpsCounter = real::ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);
	const auto pFontLiveCounter = real::ResourceManager::GetInstance().LoadFont("Lingua.otf", 16);
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
	pFpsCounter->AddComponent<real::FPSCounterComponent>();

	const auto pWorldCenter = scene.CreateGameObject();
	pWorldCenter->GetComponent<real::TransformComponent>()->SetLocalPosition({ 640 / 2.f, 300 });

	const auto pCharacter = pWorldCenter->CreateGameObject();
	pCharacter->GetComponent<real::TransformComponent>()->SetLocalPosition({ 20,0 });
	pCharacter->AddComponent<real::TextureComponent>()->SetTexture(pCharacterTexture);
	pCharacter->AddComponent<real::HealthComponent>()->SetHealth(3);
	pCharacter->GetComponent<real::HealthComponent>()->SetLives(3);
	pCharacter->GetComponent<real::HealthComponent>()->SetSpawnPoint(pCharacter->GetComponent<real::TransformComponent>()->GetWorldPosition());

	const auto pLiveCounterP1 = pCharacter->CreateGameObject();
	pLiveCounterP1->GetComponent<real::TransformComponent>()->SetLocalPosition(-338, 100);
	pLiveCounterP1->AddComponent<real::TextureComponent>();
	pLiveCounterP1->AddComponent<real::TextComponent>()->SetColor(254, 194, 12);
	pLiveCounterP1->GetComponent<real::TextComponent>()->SetFont(pFontLiveCounter);
	pLiveCounterP1->GetComponent<real::TextComponent>()->SetText("Lives: 3");
	pLiveCounterP1->AddObserver<real::PlayerData>();

	const auto pPointCounterP1 = pCharacter->CreateGameObject();
	pPointCounterP1->GetComponent<real::TransformComponent>()->SetLocalPosition(-338, 118);
	pPointCounterP1->AddComponent<real::TextureComponent>();
	pPointCounterP1->AddComponent<real::TextComponent>()->SetColor(254, 194, 12);
	pPointCounterP1->GetComponent<real::TextComponent>()->SetFont(pFontLiveCounter);
	pPointCounterP1->GetComponent<real::TextComponent>()->SetText("Points: 0");
	pPointCounterP1->AddObserver<real::PlayerData>();

	const auto pSecondaryCharacter = pCharacter->CreateGameObject();
	pSecondaryCharacter->GetComponent<real::TransformComponent>()->SetLocalPosition({ 20,0 });
	pSecondaryCharacter->AddComponent<real::TextureComponent>()->SetTexture(pSecondaryCharacterTexture);
	pSecondaryCharacter->AddComponent<real::HealthComponent>()->SetHealth(3);
	pSecondaryCharacter->GetComponent<real::HealthComponent>()->SetLives(3);
	pSecondaryCharacter->GetComponent<real::HealthComponent>()->SetSpawnPoint(pSecondaryCharacter->GetComponent<real::TransformComponent>()->GetWorldPosition());

	const auto pLiveCounterP2 = pSecondaryCharacter->CreateGameObject();
	pLiveCounterP2->GetComponent<real::TransformComponent>()->SetLocalPosition(-358, 140);
	pLiveCounterP2->AddComponent<real::TextureComponent>();
	pLiveCounterP2->AddComponent<real::TextComponent>()->SetColor(50, 205, 50);
	pLiveCounterP2->GetComponent<real::TextComponent>()->SetFont(pFontLiveCounter);
	pLiveCounterP2->GetComponent<real::TextComponent>()->SetText("Lives: 3");
	pLiveCounterP2->AddObserver<real::PlayerData>();

	const auto pPointCounterP2 = pSecondaryCharacter->CreateGameObject();
	pPointCounterP2->GetComponent<real::TransformComponent>()->SetLocalPosition(-358, 158);
	pPointCounterP2->AddComponent<real::TextureComponent>();
	pPointCounterP2->AddComponent<real::TextComponent>()->SetColor(50, 205, 50);
	pPointCounterP2->GetComponent<real::TextComponent>()->SetFont(pFontLiveCounter);
	pPointCounterP2->GetComponent<real::TextComponent>()->SetText("Points: 0");
	pPointCounterP2->AddObserver<real::PlayerData>();


	input.EnableCoOp(true);
	const std::vector<int> controllerIdcs = input.AddControllers();

	if (controllerIdcs.size() == 0)
	{
		// only keyboard
		input.UseKeyboard(true);
		input.AddKeyboardCommands<real::MoveCommand>(SDL_SCANCODE_A, KEYPRESSED, pCharacter);
		input.AddKeyboardCommands<real::MoveCommand>(SDL_SCANCODE_D, KEYPRESSED, pCharacter);
		input.AddKeyboardCommands<real::MoveCommand>(SDL_SCANCODE_S, KEYPRESSED, pCharacter);
		input.AddKeyboardCommands<real::MoveCommand>(SDL_SCANCODE_W, KEYPRESSED, pCharacter);
		input.AddKeyboardCommands<real::DamageCommand>(SDL_SCANCODE_SPACE, SDL_KEYUP, pLiveCounterP1);
		input.AddKeyboardCommands<real::AddPointsCommand>(SDL_SCANCODE_LSHIFT, SDL_KEYUP, pPointCounterP1);
	}
	else if (controllerIdcs.size() >= 1)
	{
		using ControllerButton = real::XInputController::ControllerButton;
		using InputType = real::XInputController::InputType;

		// 1 controller (and 1 keyboard if co-op)
		input.AddControllerCommands<real::MoveCommand>(ControllerButton::LeftThumb, InputType::leftThumbMoved, controllerIdcs[0], pCharacter);
		input.AddControllerCommands<real::TestCommand>(ControllerButton::ButtonDown, InputType::down, controllerIdcs[0], pCharacter);
		input.AddControllerCommands<real::DamageCommand>(ControllerButton::ButtonLeft, InputType::down, controllerIdcs[0], pLiveCounterP1);
		input.AddControllerCommands<real::AddPointsCommand>(ControllerButton::ButtonUp, InputType::down, controllerIdcs[0], pPointCounterP1);

		if (input.IsCoOpEnabled() == true)
		{
			if (controllerIdcs.size() == 2)
			{
				input.UseKeyboard(false);

				input.AddControllerCommands<real::MoveCommand>(ControllerButton::RightThumb, InputType::leftThumbMoved, controllerIdcs[1], pSecondaryCharacter, 100.f);
				input.AddControllerCommands<real::DamageCommand>(ControllerButton::ButtonLeft, InputType::down, controllerIdcs[1], pLiveCounterP2);
				input.AddControllerCommands<real::AddPointsCommand>(ControllerButton::ButtonUp, InputType::down, controllerIdcs[1], pPointCounterP2);
			}
			else
			{
				input.UseKeyboard(true);
				input.AddKeyboardCommands<real::MoveCommand>(SDL_SCANCODE_A, KEYPRESSED, pSecondaryCharacter, 100.f);
				input.AddKeyboardCommands<real::MoveCommand>(SDL_SCANCODE_D, KEYPRESSED, pSecondaryCharacter, 100.f);
				input.AddKeyboardCommands<real::MoveCommand>(SDL_SCANCODE_S, KEYPRESSED, pSecondaryCharacter, 100.f);
				input.AddKeyboardCommands<real::MoveCommand>(SDL_SCANCODE_W, KEYPRESSED, pSecondaryCharacter, 100.f);
				input.AddKeyboardCommands<real::DamageCommand>(SDL_SCANCODE_SPACE, SDL_KEYUP, pLiveCounterP2);
				input.AddKeyboardCommands<real::AddPointsCommand>(SDL_SCANCODE_LSHIFT, SDL_KEYUP, pPointCounterP2);
			}
		}
		else
			input.UseKeyboard(false);
	}

	achievement.steamAchievements = new real::SteamAchievements(achievement.achievements.data(), 4);
}

int main(int, char* [])
{
	if (!SteamAPI_Init())
	{
		std::cerr << "Fatal Error - Steam must be running to play this game (SteamAPI_Init() failed)." << '\n';
		return 1;
	}
	else
		std::cout << "Successfully initialized steam." << '\n';

	real::Minigin engine("../Data/");
	engine.Run(load);

	SteamAPI_Shutdown();
	return 0;
};