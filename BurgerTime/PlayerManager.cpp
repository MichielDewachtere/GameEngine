#include "PlayerManager.h"

#include <ColliderComponent.h>
#include <Input.h>
#include <InputMap.h>
#include <Locator.h>
#include <Logger.h>
#include <SpriteComponent.h>
#include <TextureComponent.h>
#include <TransformComponent.h>
#include <TextComponent.h>


#include "HealthComponent.h"
#include "PlayerCharacter.h"

#include "GameInfo.h"
#include "HighScoreParser.h"

#include "HighScoreDisplay.h"
#include "LevelDisplay.h"
#include "LivesDisplay.h"
#include "PepperCounter.h"
#include "ScoreDisplay.h"

#include "LoadNextLevelCommand.h"
#include "MoveCommand.h"
#include "MuteCommand.h"
#include "StunCommand.h"

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

	if (scene.GetName() == Scenes::level01 && m_GameHasBegun == false)
	{
		InitHud();
		m_pHud->Start();

		for (const auto& pPlayer : m_PlayerPtrs)
		{
			pPlayer->Start();
		}
	}

	++m_CurrentLevel;

	scene.Add(m_pHud);

	for (size_t i = 0; i < m_PlayerPtrs.size(); ++i)
	{
		const auto playerPos = scene.FindObjectsWithTag(Tags::player_spawn)[i]->GetComponent<real::TransformComponent>()->GetWorldPosition();
		m_PlayerPtrs[i]->GetComponent<real::TransformComponent>()->SetWorldPosition(playerPos);
		m_PlayerPtrs[i]->GetComponent<HealthComponent>()->SetSpawnPoint(playerPos);
		m_PlayerPtrs[i]->GetComponent<real::SpriteComponent>()->SelectSprite(1);

		scene.Add(m_PlayerPtrs[i]);
	}

	m_AmountOfPlates = 0;
	for (const auto& pIngredient : scene.FindObjectsWithTag(Tags::ingredient))
	{
		++m_AmountOfPlates;
		pIngredient->GetComponent<Ingredient>()->landedOnPlate.AddObserver(this);
	}

	playerStopMoving.Notify(false);

	m_GameHasBegun = true;
	real::Locator::GetAudioSystem().Play(Sounds::level_intro);
	real::Locator::GetAudioSystem().Play(Sounds::background);
}

void PlayerManager::HandleEvent(Ingredient& ingredient)
{
	--m_AmountOfPlates;

	ingredient.landedOnPlate.RemoveObserver(this);

	if (m_AmountOfPlates == 0)
		PlayerWins();
}

void PlayerManager::AddPlayer(bool useKeyboard, const int controllerIdx)
{
	if (m_ControllersInUse.empty() == false && std::ranges::find(m_ControllersInUse, controllerIdx) != m_ControllersInUse.end())
	{
		real::Logger::LogWarning({"Controller {} has already been assigned to a character"}, controllerIdx);
		return;
	}

	auto& input = real::Input::GetInstance();
	real::InputMap* gamePlayInputMap;

	std::shared_ptr<real::Texture2D> pCharacterSpriteSheetTexture;
	real::SpriteSheet spriteSheet;
	spriteSheet.columns = 9;
	spriteSheet.rows = 2;
	spriteSheet.animTimer = 1 / 10.f;

	if (m_PlayerPtrs.empty())
	{
		pCharacterSpriteSheetTexture = real::Locator::GetResourceSystem().LoadTexture("characters/PeterPepperSpriteSheet.png");
		spriteSheet.pTexture = pCharacterSpriteSheetTexture;

		gamePlayInputMap = input.AddInputMap(InputMaps::gameplay);
		real::Logger::LogInfo({"Player 1 has been initialized"});
		playerJoined.Notify(1);
	}
	else if (m_PlayerPtrs.size() == 1)
	{
		if (useKeyboard && m_KeyboardInUse)
		{
			real::Logger::LogWarning({"The keyboard is already linked to an existing player"});
			return;
		}

		input.EnableCoOp(true);
		pCharacterSpriteSheetTexture = real::Locator::GetResourceSystem().LoadTexture("characters/SallySaltSpriteSheet.png");
		spriteSheet.pTexture = pCharacterSpriteSheetTexture;

		gamePlayInputMap = input.GetInputMap(InputMaps::gameplay);
		real::Logger::LogInfo({"Player 2 has been initialized"});
		playerJoined.Notify(2);
	}
	else
	{
		real::Logger::LogWarning({"There are already 2 players initialized"});
		return;
	}

	const auto pPepperTexture = real::Locator::GetResourceSystem().LoadTexture("characters/pepper.png");

	const auto pCharacter = new real::GameObject(nullptr, Tags::player);
	//pCharacter->SetTag(Tags::player);
	pCharacter->SetCanBeDestroyed(false);
	pCharacter->AddComponent<real::TransformComponent>()->SetLocalPosition(288, 423);
	pCharacter->AddComponent<real::SpriteComponent>(spriteSheet)->SelectSprite(1);
	const auto spriteSize = pCharacter->GetComponent<real::SpriteComponent>()->GetSpriteSize();
	pCharacter->AddComponent<HealthComponent>()->SetLives(4);
	pCharacter->GetComponent<HealthComponent>()->SetSpawnPoint(pCharacter->GetComponent<real::TransformComponent>()->GetWorldPosition());
	pCharacter->AddComponent<PlayerCharacter>();
	pCharacter->AddComponent<real::ColliderComponent>(spriteSize)->EnableDebugRendering(false);

	const auto pFeet = pCharacter->CreateGameObject();
	pFeet->SetTag("test");
	pFeet->SetCanBeDestroyed(false);
	pFeet->GetComponent<real::TransformComponent>()->SetLocalPosition(12, 0);
	pFeet->AddComponent<real::ColliderComponent>(glm::vec2{ 24, 48 })->EnableDebugRendering(false, Colors::purple);

	const auto pPepperArea = pCharacter->CreateGameObject();
	pPepperArea->SetCanBeDestroyed(false);
	pPepperArea->SetTag(Tags::pepper);
	pPepperArea->SetIsActive(false);
	pPepperArea->GetComponent<real::TransformComponent>()->SetIgnoreParent(true);
	pPepperArea->AddComponent<real::TextureComponent>()->SetTexture(pPepperTexture);
	pPepperArea->AddComponent<real::ColliderComponent>(pPepperTexture->GetSize())->EnableDebugRendering(false, Colors::red);

	if (useKeyboard && m_KeyboardInUse == false)
	{
		input.UseKeyboard(true);

		gamePlayInputMap->AddKeyboardInput<MoveCommand>(0, KEYPRESSED, SDL_SCANCODE_LEFT, pCharacter, glm::vec2{ -1, 0 });
		gamePlayInputMap->AddKeyboardInput<MoveCommand>(1, KEYPRESSED, SDL_SCANCODE_RIGHT, pCharacter, glm::vec2{ 1, 0 });
		gamePlayInputMap->AddKeyboardInput<MoveCommand>(2, KEYPRESSED,SDL_SCANCODE_DOWN, pCharacter, glm::vec2{ 0, -1 });
		gamePlayInputMap->AddKeyboardInput<MoveCommand>(3, KEYPRESSED,SDL_SCANCODE_UP, pCharacter, glm::vec2{ 0, 1 });
		gamePlayInputMap->AddKeyboardInput<StunCommand>(4, SDL_KEYUP, SDL_SCANCODE_Z, pCharacter);
		gamePlayInputMap->AddKeyboardInput<StunCommand>(5, SDL_KEYUP, SDL_SCANCODE_X, pCharacter);
		gamePlayInputMap->AddKeyboardInput<LoadNextLevelCommand>(6, SDL_KEYUP, SDL_SCANCODE_F1, pCharacter, Scenes::level03);
		gamePlayInputMap->AddKeyboardInput<MuteCommand>(7, SDL_KEYUP, SDL_SCANCODE_F2, nullptr);

		m_KeyboardInUse = true;
	}
	else if (useKeyboard == false)
	{
		m_ControllersInUse.push_back(controllerIdx);

		using controller_button = real::XInputController::ControllerButton;
		using input_type = real::XInputController::InputType;

		gamePlayInputMap->AddControllerInput<MoveCommand>(0, static_cast<uint8_t>(controllerIdx), controller_button::DPadLeft, input_type::pressed, pCharacter, glm::vec2{ -1, 0 });
		gamePlayInputMap->AddControllerInput<MoveCommand>(1, static_cast<uint8_t>(controllerIdx),controller_button::DPadRight, input_type::pressed, pCharacter, glm::vec2{ 1, 0 });
		gamePlayInputMap->AddControllerInput<MoveCommand>(2, static_cast<uint8_t>(controllerIdx),controller_button::DPadDown, input_type::pressed, pCharacter, glm::vec2{ 0, -1 });
		gamePlayInputMap->AddControllerInput<MoveCommand>(3, static_cast<uint8_t>(controllerIdx),controller_button::DPadUp, input_type::pressed, pCharacter, glm::vec2{ 0, 1 });
		gamePlayInputMap->AddControllerInput<StunCommand>(4, static_cast<uint8_t>(controllerIdx),controller_button::ButtonDown, input_type::down, pCharacter);
		gamePlayInputMap->AddControllerInput<StunCommand>(5, static_cast<uint8_t>(controllerIdx),controller_button::ButtonRight, input_type::down, pCharacter);
		gamePlayInputMap->AddControllerInput<LoadNextLevelCommand>(6, static_cast<uint8_t>(controllerIdx), controller_button::Back, input_type::down, pCharacter, Scenes::level03);
		gamePlayInputMap->AddControllerInput<MuteCommand>(7, static_cast<uint8_t>(controllerIdx), controller_button::ButtonLeft, input_type::down, nullptr);
	}

	m_PlayerPtrs.push_back(std::shared_ptr<real::GameObject>(pCharacter));
}

std::vector<real::GameObject*> PlayerManager::GetPlayers() const
{
	std::vector<real::GameObject*> playerPtrs;

	for (const auto& pPlayer : m_PlayerPtrs)
	{
		playerPtrs.push_back(pPlayer.get());
	}

	return playerPtrs;
}

void PlayerManager::SubmitName(std::string name)
{
	m_PlayerName = std::move(name);

	// Delete leading space
	m_PlayerName.erase(m_PlayerName.begin());
}

void PlayerManager::InitHud()
{
	const auto pFont = real::Locator::GetResourceSystem().LoadFont("fonts/8-bit-hud.ttf", 10);
	const auto pMiddleFont = real::Locator::GetResourceSystem().LoadFont("fonts/8-bit-hud.ttf", 16);
	const auto pTitleFont = real::Locator::GetResourceSystem().LoadFont("fonts/8-bit-hud.ttf", 32);

	using alignment = real::TextComponent::HorizontalAlignment;

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
	pScoreText->GetComponent<real::TextComponent>()->ChangeHorizontalAlignment(alignment::center);

	const auto pScoreCounter = m_pHud->CreateGameObject();
	pScoreCounter->GetComponent<real::TransformComponent>()->SetLocalPosition(110, 25);
	pScoreCounter->AddComponent<real::TextureComponent>();
	pScoreCounter->AddComponent<real::TextComponent>()->SetFont(pMiddleFont);
	pScoreCounter->GetComponent<real::TextComponent>()->SetText("0");
	pScoreCounter->GetComponent<real::TextComponent>()->SetColor(Colors::white);
	pScoreCounter->GetComponent<real::TextComponent>()->ChangeHorizontalAlignment(alignment::center);
	const auto pScoreDisplay = pScoreCounter->AddComponent<ScoreDisplay>();

	const auto pHighScoreText = m_pHud->CreateGameObject();
	pHighScoreText->GetComponent<real::TransformComponent>()->SetLocalPosition(280,0);
	pHighScoreText->AddComponent<real::TextureComponent>();
	pHighScoreText->AddComponent<real::TextComponent>()->SetFont(pMiddleFont);
	pHighScoreText->GetComponent<real::TextComponent>()->SetText("HI-SCORE");
	pHighScoreText->GetComponent<real::TextComponent>()->SetColor(Colors::red);
	pHighScoreText->GetComponent<real::TextComponent>()->ChangeHorizontalAlignment(alignment::center);

	const auto pHighScoreCounter = m_pHud->CreateGameObject();
	pHighScoreCounter->GetComponent<real::TransformComponent>()->SetLocalPosition(280, 25);
	pHighScoreCounter->AddComponent<real::TextureComponent>();
	pHighScoreCounter->AddComponent<real::TextComponent>()->SetFont(pMiddleFont);
	pHighScoreCounter->GetComponent<real::TextComponent>()->SetText("0");
	pHighScoreCounter->GetComponent<real::TextComponent>()->SetColor(Colors::white);
	pHighScoreCounter->GetComponent<real::TextComponent>()->ChangeHorizontalAlignment(alignment::center);
	pHighScoreCounter->AddComponent<HighScoreDisplay>(pScoreDisplay);

	const auto pPepperText = m_pHud->CreateGameObject();
	pPepperText->GetComponent<real::TransformComponent>()->SetLocalPosition(672, 0);
	pPepperText->AddComponent<real::TextureComponent>();
	pPepperText->AddComponent<real::TextComponent>()->SetFont(pMiddleFont);
	pPepperText->GetComponent<real::TextComponent>()->SetText("PEPPER");
	pPepperText->GetComponent<real::TextComponent>()->SetColor(Colors::green);
	pPepperText->GetComponent<real::TextComponent>()->ChangeHorizontalAlignment(alignment::left);

	const auto pPepperCounter = m_pHud->CreateGameObject();
	pPepperCounter->GetComponent<real::TransformComponent>()->SetLocalPosition(672, 25);
	pPepperCounter->AddComponent<real::TextureComponent>();
	pPepperCounter->AddComponent<real::TextComponent>()->SetFont(pMiddleFont);
	pPepperCounter->GetComponent<real::TextComponent>()->SetText("5");
	pPepperCounter->GetComponent<real::TextComponent>()->SetColor(Colors::white);
	pPepperCounter->GetComponent<real::TextComponent>()->ChangeHorizontalAlignment(alignment::left);
	pPepperCounter->AddComponent<PepperCounter>();

	const auto pLivesCounter = m_pHud->CreateGameObject();
	pLivesCounter->GetComponent<real::TransformComponent>()->SetLocalPosition(28, 659);
	pLivesCounter->AddComponent<LivesDisplay>();

	const auto pLevelCounter = m_pHud->CreateGameObject();
	pLevelCounter->GetComponent<real::TransformComponent>()->SetLocalPosition(672, 659);
	pLevelCounter->AddComponent<LevelDisplay>();

	m_pHud->Start();
}

void PlayerManager::PlayerWins()
{
	//TODO : set animation
	for (const auto& pPlayer: m_PlayerPtrs)
	{
		pPlayer->GetComponent<real::SpriteComponent>()->PlayAnimation({ 1,12 },-1);
	}

	real::Locator::GetAudioSystem().Stop(Sounds::level_completed.channel);
	real::Locator::GetAudioSystem().Play(Sounds::level_completed);

	levelHasEnded.Notify();
	playerStopMoving.Notify(true);

	if (m_CurrentLevel < 3)
	{
		const std::string nextLevel = "Level0" + std::to_string(m_CurrentLevel + 1);
		real::SceneManager::GetInstance().SetSceneActive(nextLevel, 4.f);
	}
	else
	{
		++m_CurrentLoop;
		m_CurrentLevel = 0;
		const std::string nextLevel = "Level0" + std::to_string(m_CurrentLevel + 1);
		real::SceneManager::GetInstance().SetSceneActive(nextLevel, 4.f);
	}
}
