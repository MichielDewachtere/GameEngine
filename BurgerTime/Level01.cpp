#include "Level01.h"

#include <SDL_scancode.h>

#include <ColliderComponent.h>
#include <TextureComponent.h>
#include <Input.h>
#include <InputMap.h>
#include <ResourceManager.h>
#include <TransformComponent.h>

#include "MoveCommand.h"
#include "LevelParser.h"
#include "HealthComponent.h"
#include "PlayerCharacter.h"
#include "StunCommand.h"
#include "GameInfo.h"
#include "LoadNextSceneCommand.h"

Level01::Level01()
	: Scene(Scenes::level01, InputMaps::gameplay)
{
}

void Level01::Load()
{
	//auto& scene = real::SceneManager::GetInstance().AddScene(Scenes::level01, InputMaps::gameplay);
	this->SetDebugRendering(true);
	//auto& input = real::Input::GetInstance();
	//const auto pInputMap = input.AddInputMap(InputMaps::gameplay);

	const auto pLevel = LevelParser::ParseLevel(*this, "Level01.json");
	pLevel->GetComponent<real::TransformComponent>()->Translate(48, 62);

#pragma region player
	//const auto pCharacterTexture = real::ResourceManager::GetInstance().LoadTexture("characters/PeterPepper.png");
	//const auto pPepperTexture = real::ResourceManager::GetInstance().LoadTexture("characters/pepper.png");

	//const auto pCharacter = pLevel->CreateGameObject();
	//pCharacter->SetTag(Tags::player);
	//pCharacter->GetComponent<real::TransformComponent>()->SetLocalPosition(288, 423);
	//pCharacter->AddComponent<real::TextureComponent>()->SetTexture(pCharacterTexture);
	//pCharacter->AddComponent<HealthComponent>()->SetLives(4);
	//pCharacter->GetComponent<HealthComponent>()->SetSpawnPoint(pCharacter->GetComponent<real::TransformComponent>()->GetWorldPosition());
	//pCharacter->AddComponent<PlayerCharacter>();
	//pCharacter->AddComponent<real::ColliderComponent>(pCharacterTexture->GetSize())->EnableDebugRendering(true);

	//const auto pFeet = pCharacter->CreateGameObject();
	////pFeet->SetTag(Tags::player);
	//pFeet->GetComponent<real::TransformComponent>()->SetLocalPosition(12, 0);
	//pFeet->AddComponent<real::ColliderComponent>(glm::vec2{ 24, 48 })->EnableDebugRendering(true, Colors::purple);
	////pFeet->GetComponent<real::ColliderComponent>()->Translate(12, 0);

	//const auto pPepperArea = pCharacter->CreateGameObject();
	//pPepperArea->SetTag(Tags::pepper);
	//pPepperArea->SetIsActive(false);
	//pPepperArea->GetComponent<real::TransformComponent>()->SetIgnoreParent(true);
	//pPepperArea->AddComponent<real::TextureComponent>()->SetTexture(pPepperTexture);
	//pPepperArea->AddComponent<real::ColliderComponent>(pPepperTexture->GetSize())->EnableDebugRendering(true, Colors::red);

#pragma endregion player

	//input.EnableCoOp(true);
	//const auto controllerIdcs = input.AddControllers();

	//if (controllerIdcs.empty())
	//{
	//	// only keyboard
	//	input.UseKeyboard(true);
	//	pInputMap->AddKeyboardCommands<MoveCommand>(SDL_SCANCODE_LEFT, KEYPRESSED, pCharacter, glm::vec2{ -1, 0 });
	//	pInputMap->AddKeyboardCommands<MoveCommand>(SDL_SCANCODE_RIGHT, KEYPRESSED, pCharacter, glm::vec2{ 1, 0 });
	//	pInputMap->AddKeyboardCommands<MoveCommand>(SDL_SCANCODE_DOWN, KEYPRESSED, pCharacter, glm::vec2{ 0, -1 });
	//	pInputMap->AddKeyboardCommands<MoveCommand>(SDL_SCANCODE_UP, KEYPRESSED, pCharacter, glm::vec2{ 0, 1 });
	//	pInputMap->AddKeyboardCommands<StunCommand>(SDL_SCANCODE_Z, SDL_KEYUP, pCharacter);
	//	pInputMap->AddKeyboardCommands<StunCommand>(SDL_SCANCODE_X, SDL_KEYUP, pCharacter);
	//	pInputMap->AddKeyboardCommands<LoadNextSceneCommand>(SDL_SCANCODE_F1, SDL_KEYUP, pCharacter, Scenes::level02);
	//}
	//else if (!controllerIdcs.empty())
	//{
	//	using ControllerButton = real::XInputController::ControllerButton;
	//	using InputType = real::XInputController::InputType;

	//	// 1 controller (and 1 keyboard if co-op)
	//	pInputMap->AddControllerCommands<MoveCommand>(ControllerButton::DPadLeft, InputType::pressed, controllerIdcs[0], pCharacter, glm::vec2{ -1, 0 }, 25.f);
	//	pInputMap->AddControllerCommands<MoveCommand>(ControllerButton::DPadRight, InputType::pressed, controllerIdcs[0], pCharacter, glm::vec2{ 1, 0 }, 25.f);
	//	pInputMap->AddControllerCommands<MoveCommand>(ControllerButton::DPadUp, InputType::pressed, controllerIdcs[0], pCharacter, glm::vec2{ 0, 1 }, 25.f);
	//	pInputMap->AddControllerCommands<MoveCommand>(ControllerButton::DPadDown, InputType::pressed, controllerIdcs[0], pCharacter, glm::vec2{ 0, -1 }, 25.f);
	//	pInputMap->AddControllerCommands<real::TestCommand>(ControllerButton::ButtonRight, InputType::pressed, controllerIdcs[0], pCharacter);
	//	pInputMap->AddControllerCommands<real::TestCommand>(ControllerButton::ButtonDown, InputType::pressed, controllerIdcs[0], pCharacter);

		//if (input.IsCoOpEnabled() == true)
		//{
		//	if (controllerIdcs.size() == 2)
		//	{
		//		input.UseKeyboard(false);
		//		
		//		pInputMap->AddControllerCommands<MoveCommand>(ControllerButton::DPadLeft, InputType::pressed, controllerIdcs[1], pSecondaryCharacter, glm::vec2{ -1,0 }, 100.f);
		//		pInputMap->AddControllerCommands<MoveCommand>(ControllerButton::DPadRight, InputType::pressed, controllerIdcs[1], pSecondaryCharacter, glm::vec2{ 1,0 }, 100.f);
		//		pInputMap->AddControllerCommands<MoveCommand>(ControllerButton::DPadUp, InputType::pressed, controllerIdcs[1], pSecondaryCharacter, glm::vec2{ 0,1 }, 100.f);
		//		pInputMap->AddControllerCommands<MoveCommand>(ControllerButton::DPadDown, InputType::pressed, controllerIdcs[1], pSecondaryCharacter, glm::vec2{ 0,-1 }, 100.f);
		//		pInputMap->AddControllerCommands<DamageCommand>(ControllerButton::ButtonLeft, InputType::down, controllerIdcs[1], pSecondaryCharacter);
		//		pInputMap->AddControllerCommands<real::AddPointsCommand>(ControllerButton::ButtonUp, InputType::down, controllerIdcs[1], pSecondaryCharacter);
		//	}
		//	else
		//	{
		//		input.UseKeyboard(true);
		//		pInputMap->AddKeyboardCommands<MoveCommand>(SDL_SCANCODE_A, KEYPRESSED, pSecondaryCharacter, glm::vec2{ -1,0 }, 100.f);
		//		pInputMap->AddKeyboardCommands<MoveCommand>(SDL_SCANCODE_D, KEYPRESSED, pSecondaryCharacter, glm::vec2{ 1,0 }, 100.f);
		//		pInputMap->AddKeyboardCommands<MoveCommand>(SDL_SCANCODE_S, KEYPRESSED, pSecondaryCharacter, glm::vec2{ 0,-1 }, 100.f);
		//		pInputMap->AddKeyboardCommands<MoveCommand>(SDL_SCANCODE_W, KEYPRESSED, pSecondaryCharacter, glm::vec2{ 0,1 }, 100.f);
		//		pInputMap->AddKeyboardCommands<DamageCommand>(SDL_SCANCODE_SPACE, SDL_KEYUP, pSecondaryCharacter);
		//		pInputMap->AddKeyboardCommands<real::AddPointsCommand>(SDL_SCANCODE_LSHIFT, SDL_KEYUP, pSecondaryCharacter);
		//	}
		//}
		//else
		//	input.UseKeyboard(false);
	//}

	m_IsLoaded = true;
}
