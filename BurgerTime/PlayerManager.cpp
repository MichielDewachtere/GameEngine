#include "PlayerManager.h"

#include <ColliderComponent.h>
#include <Input.h>
#include <InputMap.h>
#include <ResourceManager.h>
#include <TextureComponent.h>
#include <TransformComponent.h>

#include "GameInfo.h"
#include "HealthComponent.h"
#include "LoadNextSceneCommand.h"
#include "MoveCommand.h"
#include "PlayerCharacter.h"
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

	for (const auto& pPlayer : m_PlayerPtrs)
	{
		const auto playerPos = scene.FindObjectsWithTag(Tags::player_spawn)[0]->GetComponent<real::TransformComponent>()->GetWorldPosition();
		pPlayer->GetComponent<real::TransformComponent>()->SetWorldPosition(playerPos);
		scene.Add(pPlayer);
	}
}

void PlayerManager::AddPlayer(bool useKeyboard)
{
	auto& input = real::Input::GetInstance();
	const auto pInputMap = input.AddInputMap(InputMaps::gameplay);

	std::shared_ptr<real::Texture2D> pCharacterTexture;

	if (m_PlayerPtrs.empty())
		pCharacterTexture = real::ResourceManager::GetInstance().LoadTexture("characters/PeterPepper.png");
	else
		pCharacterTexture = real::ResourceManager::GetInstance().LoadTexture("characters/SallySalt.png");

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


	if (useKeyboard)
	{
		input.UseKeyboard(true);
		pInputMap->AddKeyboardCommands<MoveCommand>(SDL_SCANCODE_LEFT, KEYPRESSED, pCharacter, glm::vec2{ -1, 0 });
		pInputMap->AddKeyboardCommands<MoveCommand>(SDL_SCANCODE_RIGHT, KEYPRESSED, pCharacter, glm::vec2{ 1, 0 });
		pInputMap->AddKeyboardCommands<MoveCommand>(SDL_SCANCODE_DOWN, KEYPRESSED, pCharacter, glm::vec2{ 0, -1 });
		pInputMap->AddKeyboardCommands<MoveCommand>(SDL_SCANCODE_UP, KEYPRESSED, pCharacter, glm::vec2{ 0, 1 });
		pInputMap->AddKeyboardCommands<StunCommand>(SDL_SCANCODE_Z, SDL_KEYUP, pCharacter);
		pInputMap->AddKeyboardCommands<StunCommand>(SDL_SCANCODE_X, SDL_KEYUP, pCharacter);
		pInputMap->AddKeyboardCommands<LoadNextSceneCommand>(SDL_SCANCODE_F1, SDL_KEYUP, pCharacter, Scenes::level03);
	}

	m_PlayerPtrs.push_back(std::shared_ptr<real::GameObject>(pCharacter));
}
