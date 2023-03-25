#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "FPSCounterComponent.h"
#include "GameObject.h"
#include "Input.h"
#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "RotatorComponent.h"
#include "Scene.h"
#include "TextureComponent.h"
#include "TextComponent.h"
#include "TransformComponent.h"
#include "Command.h"

void load()
{
	auto& scene = real::SceneManager::GetInstance().CreateScene("Demo");
	auto& input = real::Input::GetInstance();

	const auto pBackgroundTexture = real::ResourceManager::GetInstance().LoadTexture("background.tga");
	const auto pFontTitle = real::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	const auto pFontFpsCounter = real::ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);
	const auto pLogoTexture = real::ResourceManager::GetInstance().LoadTexture("logo.tga");
	const auto pCharacterTexture = real::ResourceManager::GetInstance().LoadTexture("Character.png");
	const auto pSecondaryCharacterTexture = real::ResourceManager::GetInstance().LoadTexture("SecondaryCharacter.png");

	const auto pBackground = scene.CreateGameObject();
	pBackground->GetComponent<real::TransformComponent>()->SetLocalPosition(0, 0);
	pBackground->AddComponent<real::TextureComponent>()->SetTexture(pBackgroundTexture);

	const auto pTitle = scene.CreateGameObject();
	pTitle->GetComponent<real::TransformComponent>()->SetLocalPosition(80, 60);
	pTitle->AddComponent<real::TextureComponent>();
	pTitle->AddComponent<real::TextComponent>()->SetText("Programming 4 Assignment");
	pTitle->GetComponent<real::TextComponent>()->SetFont(pFontTitle);

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
	pWorldCenter->GetComponent<real::TransformComponent>()->SetLocalPosition({ 640 / 2.f, 300,0 });

	const auto pCharacter = pWorldCenter->CreateGameObject();
	pCharacter->GetComponent<real::TransformComponent>()->SetLocalPosition({ 20,0,0 });
	pCharacter->AddComponent<real::TextureComponent>()->SetTexture(pCharacterTexture);

	const auto pSecondaryCharacter = pCharacter->CreateGameObject();
	pSecondaryCharacter->GetComponent<real::TransformComponent>()->SetLocalPosition({ 20,0,0 });
	pSecondaryCharacter->AddComponent<real::TextureComponent>()->SetTexture(pSecondaryCharacterTexture);

	input.EnableCoOp(true);
	const std::vector<int> controllerIdcs = input.AddControllers();
	
	if (controllerIdcs.size() == 0)
	{
		// only keyboard
		input.UseKeyboard(true);
	}
	else if (controllerIdcs.size() >= 1)
	{
		// 1 controller (and 1 keyboard if co-op)
		input.AddControllerCommands<real::MoveCommand>(real::XInputController::ControllerButton::LeftThumb, real::XInputController::InputType::leftThumbMoved, controllerIdcs[0], pCharacter);
		input.AddControllerCommands<real::TestCommand>(real::XInputController::ControllerButton::ButtonDown, real::XInputController::InputType::down, controllerIdcs[0], pCharacter);

		if (input.IsCoOpEnabled() == true)
		{
			if (controllerIdcs.size() == 2)
			{
				input.AddControllerCommands<real::MoveCommand>(real::XInputController::ControllerButton::RightThumb, real::XInputController::InputType::leftThumbMoved, controllerIdcs[1], pSecondaryCharacter, 100.f);
				input.UseKeyboard(false);
			}
			else
			{
				input.UseKeyboard(true);
				input.AddKeyboardCommands<real::MoveCommand>(SDL_SCANCODE_A, KEYPRESSED, pSecondaryCharacter, 100.f);
				input.AddKeyboardCommands<real::MoveCommand>(SDL_SCANCODE_D, KEYPRESSED, pSecondaryCharacter, 100.f);
				input.AddKeyboardCommands<real::MoveCommand>(SDL_SCANCODE_S, KEYPRESSED, pSecondaryCharacter, 100.f);
				input.AddKeyboardCommands<real::MoveCommand>(SDL_SCANCODE_W, KEYPRESSED, pSecondaryCharacter, 100.f);
			}
		}
		else
			input.UseKeyboard(false);
	}
}

int main(int, char* []) {
	real::Minigin engine("../Data/");
	engine.Run(load);
	return 0;
};