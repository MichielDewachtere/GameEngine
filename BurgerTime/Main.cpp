#include "stdafx.h"

#include <Minigin.h> 

#include <GameObject.h>

#include <InputMap.h>

#include <SceneManager.h>
#include <Scene.h>

#include <ResourceManager.h>

#include <TextComponent.h>
#include <TextureComponent.h>
#include <TransformComponent.h>
#include <ColliderComponent.h>

#ifdef USE_STEAM
#include <Achievements.h>
#endif 

#include "FPSCounterComponent.h"
#include "HealthComponent.h"
#include "LivesDisplay.h"


#include "DamageCommand.h"
#include "GameInfo.h"
#include "LoadNextSceneCommand.h"
#include "MoveCommand.h"

real::WindowSettings g_window;

void loadControlsDisplayScene();
void loadDemoScene();
void loadLevelScene();

void load()
{
	loadControlsDisplayScene();
	loadDemoScene();
	loadLevelScene();

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

	g_window.title = "BurgerTime | Exam Assignment";
	g_window.width = 624 + 48 * 2;
	g_window.height = 640;

	real::Minigin engine("../Data/", g_window);
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
	pWorldCenter->GetComponent<real::TransformComponent>()->SetLocalPosition({ static_cast<float>(g_window.width) / 2.f, static_cast<float>(g_window.height) / 2.f });

	const auto pCharacter = pWorldCenter->CreateGameObject();
	pCharacter->GetComponent<real::TransformComponent>()->SetLocalPosition({ 20,0 });
	pCharacter->AddComponent<real::TextureComponent>()->SetTexture(pCharacterTexture);
	pCharacter->AddComponent<HealthComponent>()->SetLives(4);
	pCharacter->GetComponent<HealthComponent>()->SetSpawnPoint(pCharacter->GetComponent<real::TransformComponent>()->GetWorldPosition());

	const auto pLiveCounterP1 = pCharacter->CreateGameObject();
	pLiveCounterP1->GetComponent<real::TransformComponent>()->SetLocalPosition(-308, 100);
	pLiveCounterP1->AddComponent<real::TextureComponent>();
	pLiveCounterP1->AddComponent<real::TextComponent>()->SetColor(254, 194, 12);
	pLiveCounterP1->GetComponent<real::TextComponent>()->SetFont(pFontLiveCounter);
	pLiveCounterP1->AddComponent<LivesDisplay>(pCharacter->GetComponent<HealthComponent>());

	const auto pPointCounterP1 = pCharacter->CreateGameObject();
	pPointCounterP1->GetComponent<real::TransformComponent>()->SetLocalPosition(-308, 118);
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
	pLiveCounterP2->GetComponent<real::TransformComponent>()->SetLocalPosition(-328, 140);
	pLiveCounterP2->AddComponent<real::TextureComponent>();
	pLiveCounterP2->AddComponent<real::TextComponent>()->SetColor(50, 205, 50);
	pLiveCounterP2->GetComponent<real::TextComponent>()->SetFont(pFontLiveCounter);
	pLiveCounterP2->AddComponent<LivesDisplay>(pSecondaryCharacter->GetComponent<HealthComponent>());

	const auto pPointCounterP2 = pSecondaryCharacter->CreateGameObject();
	pPointCounterP2->GetComponent<real::TransformComponent>()->SetLocalPosition(-328, 158);
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
		pInputMap->AddKeyboardCommands<MoveCommand>(SDL_SCANCODE_A, KEYPRESSED, pCharacter, glm::vec2{ -1,0 });
		pInputMap->AddKeyboardCommands<MoveCommand>(SDL_SCANCODE_D, KEYPRESSED, pCharacter, glm::vec2{ 1,0 });
		pInputMap->AddKeyboardCommands<MoveCommand>(SDL_SCANCODE_S, KEYPRESSED, pCharacter, glm::vec2{ 0,-1 });
		pInputMap->AddKeyboardCommands<MoveCommand>(SDL_SCANCODE_W, KEYPRESSED, pCharacter, glm::vec2{ 0,1 });
		pInputMap->AddKeyboardCommands<DamageCommand>(SDL_SCANCODE_SPACE, SDL_KEYUP, pCharacter);
		//pInputMap->AddKeyboardCommands<real::AddPointsCommand>(SDL_SCANCODE_LSHIFT, SDL_KEYUP, pCharacter);
	}
	else if (!controllerIdcs.empty())
	{
		using ControllerButton = real::XInputController::ControllerButton;
		using InputType = real::XInputController::InputType;

		// 1 controller (and 1 keyboard if co-op)
		pInputMap->AddControllerCommands<MoveCommand>(ControllerButton::DPadLeft, InputType::pressed, controllerIdcs[0], pCharacter, glm::vec2{ -1,0 });
		pInputMap->AddControllerCommands<MoveCommand>(ControllerButton::DPadRight, InputType::pressed, controllerIdcs[0], pCharacter, glm::vec2{ 1,0 });
		pInputMap->AddControllerCommands<MoveCommand>(ControllerButton::DPadUp, InputType::pressed, controllerIdcs[0], pCharacter, glm::vec2{ 0,1 });
		pInputMap->AddControllerCommands<MoveCommand>(ControllerButton::DPadDown, InputType::pressed, controllerIdcs[0], pCharacter, glm::vec2{ 0,-1 });
		pInputMap->AddControllerCommands<real::TestCommand>(ControllerButton::ButtonDown, InputType::down, controllerIdcs[0], pCharacter);
		pInputMap->AddControllerCommands<DamageCommand>(ControllerButton::ButtonLeft, InputType::down, controllerIdcs[0], pCharacter);
		//pInputMap->AddControllerCommands<real::AddPointsCommand>(ControllerButton::ButtonUp, InputType::down, controllerIdcs[0], pCharacter);

		if (input.IsCoOpEnabled() == true)
		{
			if (controllerIdcs.size() == 2)
			{
				input.UseKeyboard(false);

				pInputMap->AddControllerCommands<MoveCommand>(ControllerButton::DPadLeft, InputType::pressed, controllerIdcs[1], pSecondaryCharacter, glm::vec2{ -1,0 }, 100.f);
				pInputMap->AddControllerCommands<MoveCommand>(ControllerButton::DPadRight, InputType::pressed, controllerIdcs[1], pSecondaryCharacter, glm::vec2{ 1,0 }, 100.f);
				pInputMap->AddControllerCommands<MoveCommand>(ControllerButton::DPadUp, InputType::pressed, controllerIdcs[1], pSecondaryCharacter, glm::vec2{ 0,1 }, 100.f);
				pInputMap->AddControllerCommands<MoveCommand>(ControllerButton::DPadDown, InputType::pressed, controllerIdcs[1], pSecondaryCharacter, glm::vec2{ 0,-1 }, 100.f);
				pInputMap->AddControllerCommands<DamageCommand>(ControllerButton::ButtonLeft, InputType::down, controllerIdcs[1], pSecondaryCharacter);
				//pInputMap->AddControllerCommands<real::AddPointsCommand>(ControllerButton::ButtonUp, InputType::down, controllerIdcs[1], pSecondaryCharacter);
			}
			else
			{
				input.UseKeyboard(true);
				pInputMap->AddKeyboardCommands<MoveCommand>(SDL_SCANCODE_A, KEYPRESSED, pSecondaryCharacter, glm::vec2{ -1,0 }, 100.f);
				pInputMap->AddKeyboardCommands<MoveCommand>(SDL_SCANCODE_D, KEYPRESSED, pSecondaryCharacter, glm::vec2{ 1,0 }, 100.f);
				pInputMap->AddKeyboardCommands<MoveCommand>(SDL_SCANCODE_S, KEYPRESSED, pSecondaryCharacter, glm::vec2{ 0,-1 }, 100.f);
				pInputMap->AddKeyboardCommands<MoveCommand>(SDL_SCANCODE_W, KEYPRESSED, pSecondaryCharacter, glm::vec2{ 0,1 }, 100.f);
				pInputMap->AddKeyboardCommands<DamageCommand>(SDL_SCANCODE_SPACE, SDL_KEYUP, pSecondaryCharacter);
				//pInputMap->AddKeyboardCommands<real::AddPointsCommand>(SDL_SCANCODE_LSHIFT, SDL_KEYUP, pSecondaryCharacter);
			}
		}
		else
			input.UseKeyboard(false);
	}

	const auto pContinueText = scene.CreateGameObject();
	pContinueText->GetComponent<real::TransformComponent>()->SetLocalPosition(320, 450);
	pContinueText->AddComponent<real::TextureComponent>();
	pContinueText->AddComponent<real::TextComponent>()->SetFont(pFontLiveCounter);
	pContinueText->GetComponent<real::TextComponent>()->SetText("To continue, press Space");

	pInputMap->AddKeyboardCommands<LoadNextSceneCommand>(SDL_SCANCODE_SPACE, SDL_KEYUP, nullptr, "level");
	pInputMap->AddControllerCommands<LoadNextSceneCommand>(real::XInputController::ControllerButton::ButtonDown, real::XInputController::InputType::down, (unsigned int)-1, nullptr, "level");
}

void loadLevelScene()
{
	auto& scene = real::SceneManager::GetInstance().CreateScene("level", "level");
	scene.SetDebugRendering(true);
	auto& input = real::Input::GetInstance();
	const auto pInputMap = input.AddInputMap("level");

	const auto pBackGroundTexture = real::ResourceManager::GetInstance().LoadTexture("Level.png");
	const auto pCharacterTexture = real::ResourceManager::GetInstance().LoadTexture("PeterPepper.png");

	const auto pBurgerTopTexture = real::ResourceManager::GetInstance().LoadTexture("Burger_BunTop.png");
	const auto pBurgerTop_LeftTexture = real::ResourceManager::GetInstance().LoadTexture("Burger/BunTop/left.png");
	const auto pBurgerTop_MiddleTexture = real::ResourceManager::GetInstance().LoadTexture("Burger/BunTop/middle.png");
	const auto pBurgerTop_RightTexture = real::ResourceManager::GetInstance().LoadTexture("Burger/BunTop/right.png");


	const auto pBurgerBottomTexture = real::ResourceManager::GetInstance().LoadTexture("Burger_BunBottom.png");
	const auto pLettuceTexture = real::ResourceManager::GetInstance().LoadTexture("Burger_Lettuce.png");
	const auto pPattyTexture = real::ResourceManager::GetInstance().LoadTexture("Burger_Patty.png");

	const auto pLevel = scene.CreateGameObject();
	pLevel->GetComponent<real::TransformComponent>()->Translate(48, 32);
	pLevel->AddComponent<real::TextureComponent>()->SetTexture(pBackGroundTexture);

	const auto pLevelBoundaries = pLevel->CreateGameObject();
	pLevelBoundaries->SetTag(Tags::boundary);
	pLevelBoundaries->GetComponent<real::TransformComponent>()->SetLocalPosition(0, -8);
	glm::vec2 levelBoundaries = { pBackGroundTexture->GetSize().x, 503 - 24 };
	pLevelBoundaries->AddComponent<real::ColliderComponent>(levelBoundaries)->SetColor(Colors::red);

	const auto pCharacter = pLevel->CreateGameObject();
	pCharacter->SetTag(Tags::player);
	pCharacter->GetComponent<real::TransformComponent>()->SetLocalPosition(288, 422);
	pCharacter->AddComponent<real::TextureComponent>()->SetTexture(pCharacterTexture);
	pCharacter->AddComponent<HealthComponent>()->SetLives(4);
	pCharacter->GetComponent<HealthComponent>()->SetSpawnPoint(pCharacter->GetComponent<real::TransformComponent>()->GetWorldPosition());
	pCharacter->AddComponent<real::ColliderComponent>(pCharacterTexture->GetSize())->EnableDebugRendering(true);

#pragma region stairs
	const auto pStair1 = pLevel->CreateGameObject();
	pStair1->SetTag(Tags::stair);
	pStair1->GetComponent<real::TransformComponent>()->SetLocalPosition(288, -8);
	pStair1->AddComponent<real::ColliderComponent>(glm::vec2{ 48,478 })->EnableDebugRendering(true, Colors::purple);

	const auto pStair2 = pLevel->CreateGameObject();
	pStair2->SetTag(Tags::stair);
	pStair2->GetComponent<real::TransformComponent>()->SetLocalPosition(144, -8);
	pStair2->AddComponent<real::ColliderComponent>(glm::vec2{ 48,478 })->EnableDebugRendering(true, Colors::purple);

	const auto pStair3 = pLevel->CreateGameObject();
	pStair3->SetTag(Tags::stair);
	pStair3->GetComponent<real::TransformComponent>()->SetLocalPosition(0, -8);
	pStair3->AddComponent<real::ColliderComponent>(glm::vec2{ 48,143 })->EnableDebugRendering(true, Colors::purple);

	const auto pStair4 = pLevel->CreateGameObject();
	pStair4->SetTag(Tags::stair);
	pStair4->GetComponent<real::TransformComponent>()->SetLocalPosition(0, 263 - 32 - 48);
	pStair4->AddComponent<real::ColliderComponent>(glm::vec2{ 48,240 + 48 })->EnableDebugRendering(true, Colors::purple);

	const auto pStair5 = pLevel->CreateGameObject();
	pStair5->SetTag(Tags::stair);
	pStair5->GetComponent<real::TransformComponent>()->SetLocalPosition(72, 167 - 32 - 48);
	pStair5->AddComponent<real::ColliderComponent>(glm::vec2{ 48,240 + 48 })->EnableDebugRendering(true, Colors::purple);

	const auto pStair6 = pLevel->CreateGameObject();
	pStair6->SetTag(Tags::stair);
	pStair6->GetComponent<real::TransformComponent>()->SetLocalPosition(216, -8);
	pStair6->AddComponent<real::ColliderComponent>(glm::vec2{ 48,143 + 48 })->EnableDebugRendering(true, Colors::purple);

	const auto pStair7 = pLevel->CreateGameObject();
	pStair7->SetTag(Tags::stair);
	pStair7->GetComponent<real::TransformComponent>()->SetLocalPosition(360, 167 - 32 - 48);
	pStair7->AddComponent<real::ColliderComponent>(glm::vec2{ 48,144 + 48 })->EnableDebugRendering(true, Colors::purple);

	const auto pStair8 = pLevel->CreateGameObject();
	pStair8->SetTag(Tags::stair);
	pStair8->GetComponent<real::TransformComponent>()->SetLocalPosition(504, 263 - 32 - 48);
	pStair8->AddComponent<real::ColliderComponent>(glm::vec2{ 48,287 })->EnableDebugRendering(true, Colors::purple);

	const auto pStair9 = pLevel->CreateGameObject();
	pStair9->SetTag(Tags::stair);
	pStair9->GetComponent<real::TransformComponent>()->SetLocalPosition(432, -8);
	pStair9->AddComponent<real::ColliderComponent>(glm::vec2{ 48,478 })->EnableDebugRendering(true, Colors::purple);

	const auto pStair10 = pLevel->CreateGameObject();
	pStair10->SetTag(Tags::stair);
	pStair10->GetComponent<real::TransformComponent>()->SetLocalPosition(576, -8);
	pStair10->AddComponent<real::ColliderComponent>(glm::vec2{ 48,191 + 48 })->EnableDebugRendering(true, Colors::purple);

	const auto pStair11 = pLevel->CreateGameObject();
	pStair11->SetTag(Tags::stair);
	pStair11->GetComponent<real::TransformComponent>()->SetLocalPosition(576, 359 - 32 - 48);
	pStair11->AddComponent<real::ColliderComponent>(glm::vec2{ 48,143 + 48 })->EnableDebugRendering(true, Colors::purple);
#pragma endregion stairs
#pragma region floors
	// ordened from top to bottom / left to right
	const auto pFloor1 = pLevel->CreateGameObject();
	pFloor1->SetTag(Tags::floor);
	pFloor1->GetComponent<real::TransformComponent>()->SetLocalPosition(0, 422);
	pFloor1->AddComponent<real::ColliderComponent>(glm::vec2{ pBackGroundTexture->GetSize().x, 48 })->EnableDebugRendering(true, Colors::yellow);

	const auto pFloor2 = pLevel->CreateGameObject();
	pFloor2->SetTag(Tags::floor);
	pFloor2->GetComponent<real::TransformComponent>()->SetLocalPosition(0, 407 - 32 - 48);
	pFloor2->AddComponent<real::ColliderComponent>(glm::vec2{ 144 * 3 + 48, 48 })->EnableDebugRendering(true, Colors::appelblauwzeegroen);

	const auto pFloor3 = pLevel->CreateGameObject();
	pFloor3->SetTag(Tags::floor);
	pFloor3->GetComponent<real::TransformComponent>()->SetLocalPosition(432, 359 - 32 - 48);
	pFloor3->AddComponent<real::ColliderComponent>(glm::vec2{ 48 + 144, 48 })->EnableDebugRendering(true, Colors::yellow);

	const auto pFloor4 = pLevel->CreateGameObject();
	pFloor4->SetTag(Tags::floor);
	pFloor4->GetComponent<real::TransformComponent>()->SetLocalPosition(144, 311 - 32 - 48);
	pFloor4->AddComponent<real::ColliderComponent>(glm::vec2{ 2 * 144 + 48, 48 })->EnableDebugRendering(true, Colors::appelblauwzeegroen);

	const auto pFloor5 = pLevel->CreateGameObject();
	pFloor5->SetTag(Tags::floor);
	pFloor5->GetComponent<real::TransformComponent>()->SetLocalPosition(0, 263 - 32 - 48);
	pFloor5->AddComponent<real::ColliderComponent>(glm::vec2{ 144 + 48, 48 })->EnableDebugRendering(true, Colors::yellow);

	const auto pFloor6 = pLevel->CreateGameObject();
	pFloor6->SetTag(Tags::floor);
	pFloor6->GetComponent<real::TransformComponent>()->SetLocalPosition(432, 263 - 32 - 48);
	pFloor6->AddComponent<real::ColliderComponent>(glm::vec2{ 144 + 48, 48 })->EnableDebugRendering(true, Colors::yellow);

	const auto pFloor7 = pLevel->CreateGameObject();
	pFloor7->SetTag(Tags::floor);
	pFloor7->GetComponent<real::TransformComponent>()->SetLocalPosition(144, 215 - 32 - 48);
	pFloor7->AddComponent<real::ColliderComponent>(glm::vec2{ 144 + 48, 48 })->EnableDebugRendering(true, Colors::appelblauwzeegroen);

	const auto pFloor8 = pLevel->CreateGameObject();
	pFloor8->SetTag(Tags::floor);
	pFloor8->GetComponent<real::TransformComponent>()->SetLocalPosition(0, 167 - 32 - 48);
	pFloor8->AddComponent<real::ColliderComponent>(glm::vec2{ 144 + 48, 48 })->EnableDebugRendering(true, Colors::yellow);

	const auto pFloor9 = pLevel->CreateGameObject();
	pFloor9->SetTag(Tags::floor);
	pFloor9->GetComponent<real::TransformComponent>()->SetLocalPosition(288, 167 - 32 - 48);
	pFloor9->AddComponent<real::ColliderComponent>(glm::vec2{ 2 * 144 + 48, 48 })->EnableDebugRendering(true, Colors::yellow);

	const auto pFloor10 = pLevel->CreateGameObject();
	pFloor10->SetTag(Tags::floor);
	pFloor10->GetComponent<real::TransformComponent>()->SetLocalPosition(0, 72 - 32 - 48);
	pFloor10->AddComponent<real::ColliderComponent>(glm::vec2{ pBackGroundTexture->GetSize().x, 48 })->EnableDebugRendering(true, Colors::appelblauwzeegroen);
#pragma endregion floors
#pragma region burgers
	const auto pBurgerTop1 = pLevel->CreateGameObject();
	pBurgerTop1->GetComponent<real::TransformComponent>()->SetLocalPosition(48, 172 - 31 - static_cast<float>(pBurgerTopTexture->GetSize().y));
	//pBurgerTop1->AddComponent<real::TextureComponent>()->SetTexture(pBurgerTopTexture);
	pBurgerTop1->AddComponent<real::ColliderComponent>(glm::vec2{ pBurgerTopTexture->GetSize().x, 48 })->EnableDebugRendering(true, Colors::red);
	pBurgerTop1->GetComponent<real::ColliderComponent>()->Translate(0, -33);

	const auto pBurgerTop1_Left = pLevel->CreateGameObject();
	pBurgerTop1_Left->GetComponent<real::TransformComponent>()->SetLocalPosition(48, 172 - 31 - static_cast<float>(pBurgerTop_LeftTexture->GetSize().y));
	pBurgerTop1_Left->AddComponent<real::TextureComponent>()->SetTexture(pBurgerTop_LeftTexture);
	pBurgerTop1_Left->AddComponent<real::ColliderComponent>(glm::vec2{ pBurgerTop_LeftTexture->GetSize().x, 48 })->EnableDebugRendering(true, Colors::red);
	pBurgerTop1_Left->GetComponent<real::ColliderComponent>()->Translate(0, -33);

	const auto pBurgerTop1_Middle = pLevel->CreateGameObject();
	pBurgerTop1_Middle->GetComponent<real::TransformComponent>()->SetLocalPosition(48 + static_cast<float>(pBurgerTop_MiddleTexture->GetSize().x), 172 - 31 - static_cast<float>(pBurgerTop_MiddleTexture->GetSize().y));
	pBurgerTop1_Middle->AddComponent<real::TextureComponent>()->SetTexture(pBurgerTop_MiddleTexture);
	pBurgerTop1_Middle->AddComponent<real::ColliderComponent>(glm::vec2{ pBurgerTop_MiddleTexture->GetSize().x, 48 })->EnableDebugRendering(true, Colors::red);
	pBurgerTop1_Middle->GetComponent<real::ColliderComponent>()->Translate(0, -33);

	const auto pBurgerTop1_Middle2 = pLevel->CreateGameObject();
	pBurgerTop1_Middle2->GetComponent<real::TransformComponent>()->SetLocalPosition(48 + static_cast<float>(pBurgerTop_MiddleTexture->GetSize().x) * 2, 172 - 31 - static_cast<float>(pBurgerTop_MiddleTexture->GetSize().y));
	pBurgerTop1_Middle2->AddComponent<real::TextureComponent>()->SetTexture(pBurgerTop_MiddleTexture);
	pBurgerTop1_Middle2->AddComponent<real::ColliderComponent>(glm::vec2{ pBurgerTop_MiddleTexture->GetSize().x, 48 })->EnableDebugRendering(true, Colors::red);
	pBurgerTop1_Middle2->GetComponent<real::ColliderComponent>()->Translate(0, -33);

	const auto pBurgerTop1_Right = pLevel->CreateGameObject();
	pBurgerTop1_Right->GetComponent<real::TransformComponent>()->SetLocalPosition(48 + static_cast<float>(pBurgerTop_RightTexture->GetSize().x) * 3, 172 - 31 - static_cast<float>(pBurgerTop_MiddleTexture->GetSize().y));
	pBurgerTop1_Right->AddComponent<real::TextureComponent>()->SetTexture(pBurgerTop_RightTexture);
	pBurgerTop1_Right->AddComponent<real::ColliderComponent>(glm::vec2{ pBurgerTop_RightTexture->GetSize().x, 48 })->EnableDebugRendering(true, Colors::red);
	pBurgerTop1_Right->GetComponent<real::ColliderComponent>()->Translate(0, -33);
#pragma endregion burgers

	input.EnableCoOp(true);
	const auto controllerIdcs = input.AddControllers();

	if (controllerIdcs.empty())
	{
		// only keyboard
		input.UseKeyboard(true);
		pInputMap->AddKeyboardCommands<MoveCommand>(SDL_SCANCODE_A, KEYPRESSED, pCharacter, glm::vec2{ -1,0 });
		pInputMap->AddKeyboardCommands<MoveCommand>(SDL_SCANCODE_D, KEYPRESSED, pCharacter, glm::vec2{ 1,0 });
		pInputMap->AddKeyboardCommands<MoveCommand>(SDL_SCANCODE_S, KEYPRESSED, pCharacter, glm::vec2{ 0,-1 });
		pInputMap->AddKeyboardCommands<MoveCommand>(SDL_SCANCODE_W, KEYPRESSED, pCharacter, glm::vec2{ 0,1 });
	}
	else if (!controllerIdcs.empty())
	{
		using ControllerButton = real::XInputController::ControllerButton;
		using InputType = real::XInputController::InputType;

		// 1 controller (and 1 keyboard if co-op)
		pInputMap->AddControllerCommands<MoveCommand>(ControllerButton::DPadLeft, InputType::pressed, controllerIdcs[0], pCharacter, glm::vec2{ -1,0 });
		pInputMap->AddControllerCommands<MoveCommand>(ControllerButton::DPadRight, InputType::pressed, controllerIdcs[0], pCharacter, glm::vec2{ 1,0 });
		pInputMap->AddControllerCommands<MoveCommand>(ControllerButton::DPadUp, InputType::pressed, controllerIdcs[0], pCharacter, glm::vec2{ 0,1 });
		pInputMap->AddControllerCommands<MoveCommand>(ControllerButton::DPadDown, InputType::pressed, controllerIdcs[0], pCharacter, glm::vec2{ 0,-1 });

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
	const auto pContinueText = scene.CreateGameObject();
	pContinueText->GetComponent<real::TransformComponent>()->SetLocalPosition(320, 450);
	pContinueText->AddComponent<real::TextureComponent>();
	pContinueText->AddComponent<real::TextComponent>()->SetFont(pFont);
	pContinueText->GetComponent<real::TextComponent>()->SetText("To continue, press Space");

	pInputMap->AddKeyboardCommands<LoadNextSceneCommand>(SDL_SCANCODE_SPACE, SDL_KEYUP, nullptr, "Demo");
	pInputMap->AddControllerCommands<LoadNextSceneCommand>(real::XInputController::ControllerButton::ButtonDown, real::XInputController::InputType::down, (unsigned int)-1, nullptr, "Demo");
}