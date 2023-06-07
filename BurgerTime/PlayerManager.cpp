#include "PlayerManager.h"

#include <ColliderComponent.h>
#include <Input.h>
#include <InputMap.h>
#include <iostream>
#include <ResourceManager.h>
#include <TextureComponent.h>
#include <TransformComponent.h>

#include "GameInfo.h"
#include "HealthComponent.h"
#include "LoadNextSceneCommand.h"
#include "Logger.h"
#include "MoveCommand.h"
#include "PlayerCharacter.h"
#include "StunCommand.h"
#include "TextComponent.h"

PlayerManager::~PlayerManager()
{
	real::SceneManager::GetInstance().onSceneLoaded.RemoveObserver(this);
}

void PlayerManager::Init()
{
	real::SceneManager::GetInstance().onSceneLoaded.AddObserver(this);
}

void PlayerManager::HandleEvent(real::Scene& scene)
{
	if (scene.GetName().find("Level") == std::string::npos)
		return;

	scene.Add(m_pHud);

	for (const auto& pPlayer : m_PlayerPtrs)
	{
		const auto playerPos = scene.FindObjectsWithTag(Tags::player_spawn)[0]->GetComponent<real::TransformComponent>()->GetWorldPosition();
		pPlayer->GetComponent<real::TransformComponent>()->SetWorldPosition(playerPos);
		scene.Add(pPlayer);
	}
}

void PlayerManager::AddPlayer(bool useKeyboard, const int controllerIdx)
{
	if (m_ControllersInUse.empty() == false && std::ranges::find(m_ControllersInUse, controllerIdx) != m_ControllersInUse.end())
	{
		real::Logger::LogWarning("Controller {} has already been assigned to a character", controllerIdx);
		return;
	}

	real::Logger::LogInfo("A player has been added");

	auto& input = real::Input::GetInstance();
	real::InputMap* pInputMap;

	std::shared_ptr<real::Texture2D> pCharacterTexture;

	if (m_PlayerPtrs.empty())
	{
		InitHud();
		pCharacterTexture = real::ResourceManager::GetInstance().LoadTexture("characters/PeterPepper.png");
		pInputMap = input.AddInputMap(InputMaps::gameplay);
	}
	else if (m_PlayerPtrs.size() == 1)
	{
		input.EnableCoOp(true);
		pCharacterTexture = real::ResourceManager::GetInstance().LoadTexture("characters/SallySalt.png");
		pInputMap = input.GetInputMap(InputMaps::gameplay);
	}
	else
	{
		real::Logger::LogWarning("There are already 2 players initialized");
		return;
	}

	const auto pPepperTexture = real::ResourceManager::GetInstance().LoadTexture("characters/pepper.png");

	const auto pCharacter = new real::GameObject();
	pCharacter->SetTag(Tags::player);
	pCharacter->SetCanBeDestroyed(false);
	pCharacter->AddComponent<real::TransformComponent>()->SetLocalPosition(288, 423);
	pCharacter->AddComponent<real::TextureComponent>()->SetTexture(pCharacterTexture);
	pCharacter->AddComponent<HealthComponent>()->SetLives(4);
	pCharacter->GetComponent<HealthComponent>()->SetSpawnPoint(pCharacter->GetComponent<real::TransformComponent>()->GetWorldPosition());
	pCharacter->AddComponent<PlayerCharacter>();
	pCharacter->AddComponent<real::ColliderComponent>(pCharacterTexture->GetSize())->EnableDebugRendering(true);

	const auto pFeet = pCharacter->CreateGameObject();
	pFeet->SetCanBeDestroyed(false);
	//pFeet->SetTag(Tags::player);
	pFeet->GetComponent<real::TransformComponent>()->SetLocalPosition(12, 0);
	pFeet->AddComponent<real::ColliderComponent>(glm::vec2{ 24, 48 })->EnableDebugRendering(true, Colors::purple);
	//pFeet->GetComponent<real::ColliderComponent>()->Translate(12, 0);

	const auto pPepperArea = pCharacter->CreateGameObject();
	pPepperArea->SetCanBeDestroyed(false);
	pPepperArea->SetTag(Tags::pepper);
	pPepperArea->SetIsActive(false);
	pPepperArea->GetComponent<real::TransformComponent>()->SetIgnoreParent(true);
	pPepperArea->AddComponent<real::TextureComponent>()->SetTexture(pPepperTexture);
	pPepperArea->AddComponent<real::ColliderComponent>(pPepperTexture->GetSize())->EnableDebugRendering(true, Colors::red);

	if (useKeyboard && m_KeyboardInUse == false)
	{
		input.UseKeyboard(true);
		pInputMap->AddKeyboardCommands<MoveCommand>(SDL_SCANCODE_LEFT, KEYPRESSED, pCharacter, glm::vec2{ -1, 0 });
		pInputMap->AddKeyboardCommands<MoveCommand>(SDL_SCANCODE_RIGHT, KEYPRESSED, pCharacter, glm::vec2{ 1, 0 });
		pInputMap->AddKeyboardCommands<MoveCommand>(SDL_SCANCODE_DOWN, KEYPRESSED, pCharacter, glm::vec2{ 0, -1 });
		pInputMap->AddKeyboardCommands<MoveCommand>(SDL_SCANCODE_UP, KEYPRESSED, pCharacter, glm::vec2{ 0, 1 });
		pInputMap->AddKeyboardCommands<StunCommand>(SDL_SCANCODE_Z, SDL_KEYUP, pCharacter);
		pInputMap->AddKeyboardCommands<StunCommand>(SDL_SCANCODE_X, SDL_KEYUP, pCharacter);
		pInputMap->AddKeyboardCommands<LoadNextSceneCommand>(SDL_SCANCODE_F1, SDL_KEYUP, pCharacter, Scenes::level03);

		m_KeyboardInUse = true;
	}
	else if (useKeyboard == false)
	{
		m_ControllersInUse.push_back(controllerIdx);

		using controller_button = real::XInputController::ControllerButton;
		using input_type = real::XInputController::InputType;

		pInputMap->AddControllerCommands<MoveCommand>(controller_button::DPadLeft, input_type::pressed, controllerIdx, pCharacter, glm::vec2{ -1, 0 });
		pInputMap->AddControllerCommands<MoveCommand>(controller_button::DPadRight, input_type::pressed, controllerIdx, pCharacter, glm::vec2{ 1, 0 });
		pInputMap->AddControllerCommands<MoveCommand>(controller_button::DPadDown, input_type::pressed, controllerIdx, pCharacter, glm::vec2{ 0, -1 });
		pInputMap->AddControllerCommands<MoveCommand>(controller_button::DPadUp, input_type::pressed, controllerIdx, pCharacter, glm::vec2{ 0, 1 });
		pInputMap->AddControllerCommands<StunCommand>(controller_button::ButtonDown, input_type::down, controllerIdx, pCharacter);
		pInputMap->AddControllerCommands<StunCommand>(controller_button::ButtonLeft, input_type::down, controllerIdx, pCharacter);
		pInputMap->AddControllerCommands<LoadNextSceneCommand>(controller_button::Back, input_type::down, controllerIdx, pCharacter, Scenes::level03);
	}
	
	m_PlayerPtrs.push_back(std::shared_ptr<real::GameObject>(pCharacter));
}

void PlayerManager::InitHud()
{
	const auto pFont = real::ResourceManager::GetInstance().LoadFont("8-bit-hud.ttf", 10);
	const auto pMiddleFont = real::ResourceManager::GetInstance().LoadFont("8-bit-hud.ttf", 16);
	const auto pTitleFont = real::ResourceManager::GetInstance().LoadFont("8-bit-hud.ttf", 32);

	using alignment = real::TextComponent::Alignment;

	m_pHud = std::make_shared<real::GameObject>();
	m_pHud->SetTag("HUD");
	m_pHud->SetCanBeDestroyed(false);
	m_pHud->AddComponent<real::TransformComponent>();

	const auto pScoreText = m_pHud->CreateGameObject();
	pScoreText->GetComponent<real::TransformComponent>()->SetLocalPosition(110,0);
	pScoreText->AddComponent<real::TextureComponent>();
	pScoreText->AddComponent<real::TextComponent>()->SetFont(pMiddleFont);
	pScoreText->GetComponent<real::TextComponent>()->SetText("1UP");
	pScoreText->GetComponent<real::TextComponent>()->SetColor(Colors::red);
	pScoreText->GetComponent<real::TextComponent>()->ChangeAlignment(alignment::center);

	const auto pScoreCounter = m_pHud->CreateGameObject();
	pScoreCounter->GetComponent<real::TransformComponent>()->SetLocalPosition(110, 25);
	pScoreCounter->AddComponent<real::TextureComponent>();
	pScoreCounter->AddComponent<real::TextComponent>()->SetFont(pMiddleFont);
	pScoreCounter->GetComponent<real::TextComponent>()->SetText("0");
	pScoreCounter->GetComponent<real::TextComponent>()->SetColor(Colors::white);
	pScoreCounter->GetComponent<real::TextComponent>()->ChangeAlignment(alignment::center);

	const auto pHighScoreText = m_pHud->CreateGameObject();
	pHighScoreText->GetComponent<real::TransformComponent>()->SetLocalPosition(280,0);
	pHighScoreText->AddComponent<real::TextureComponent>();
	pHighScoreText->AddComponent<real::TextComponent>()->SetFont(pMiddleFont);
	pHighScoreText->GetComponent<real::TextComponent>()->SetText("HI-SCORE");
	pHighScoreText->GetComponent<real::TextComponent>()->SetColor(Colors::red);
	pHighScoreText->GetComponent<real::TextComponent>()->ChangeAlignment(alignment::center);

	const auto pHighScoreCounter = m_pHud->CreateGameObject();
	pHighScoreCounter->GetComponent<real::TransformComponent>()->SetLocalPosition(280, 25);
	pHighScoreCounter->AddComponent<real::TextureComponent>();
	pHighScoreCounter->AddComponent<real::TextComponent>()->SetFont(pMiddleFont);
	pHighScoreCounter->GetComponent<real::TextComponent>()->SetText("0");
	pHighScoreCounter->GetComponent<real::TextComponent>()->SetColor(Colors::white);
	pHighScoreCounter->GetComponent<real::TextComponent>()->ChangeAlignment(alignment::center);

	const auto pPepperText = m_pHud->CreateGameObject();
	pPepperText->GetComponent<real::TransformComponent>()->SetLocalPosition(672, 0);
	pPepperText->AddComponent<real::TextureComponent>();
	pPepperText->AddComponent<real::TextComponent>()->SetFont(pMiddleFont);
	pPepperText->GetComponent<real::TextComponent>()->SetText("PEPPER");
	pPepperText->GetComponent<real::TextComponent>()->SetColor(Colors::green);
	pPepperText->GetComponent<real::TextComponent>()->ChangeAlignment(alignment::left);

	const auto pPepperCounter = m_pHud->CreateGameObject();
	pPepperCounter->GetComponent<real::TransformComponent>()->SetLocalPosition(672, 25);
	pPepperCounter->AddComponent<real::TextureComponent>();
	pPepperCounter->AddComponent<real::TextComponent>()->SetFont(pMiddleFont);
	pPepperCounter->GetComponent<real::TextComponent>()->SetText("0");
	pPepperCounter->GetComponent<real::TextComponent>()->SetColor(Colors::white);
	pPepperCounter->GetComponent<real::TextComponent>()->ChangeAlignment(alignment::left);
}