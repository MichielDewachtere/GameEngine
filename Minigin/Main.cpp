#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "FPSCounterComponent.h"
#include "GameObject.h"
#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "TextureComponent.h"
#include "TextComponent.h"
#include "TransformComponent.h"

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	const auto pBackgroundTexture = dae::ResourceManager::GetInstance().LoadTexture("background.tga");
	const auto pFontTitle = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	const auto pFontFpsCounter = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);
	const auto pLogoTexture = dae::ResourceManager::GetInstance().LoadTexture("logo.tga");

	const auto pBackground = std::make_shared<dae::GameObject>();
	pBackground->AddComponent<dae::TransformComponent>()->SetPosition(0, 0);
	pBackground->AddComponent<dae::TextureComponent>()->SetTexture(pBackgroundTexture);
	scene.Add(pBackground);

	const auto pTitle = std::make_shared<dae::GameObject>();
	pTitle->AddComponent<dae::TransformComponent>()->SetPosition(80, 60);
	pTitle->AddComponent<dae::TextureComponent>();
	pTitle->AddComponent<dae::TextComponent>()->SetText("Programming 4 Assignment");
	pTitle->GetComponent<dae::TextComponent>()->SetFont(pFontTitle);
	scene.Add(pTitle);

	const auto pLogo = std::make_shared<dae::GameObject>();
	pLogo->AddComponent<dae::TransformComponent>()->SetPosition(216, 180);
	pLogo->AddComponent<dae::TextureComponent>()->SetTexture(pLogoTexture);
	scene.Add(pLogo);

	const auto pFpsCounter = std::make_shared<dae::GameObject>();
	pFpsCounter->AddComponent<dae::TransformComponent>()->SetPosition(2, 2);
	pFpsCounter->AddComponent<dae::TextureComponent>();
	pFpsCounter->AddComponent<dae::TextComponent>()->SetFont(pFontFpsCounter);
	pFpsCounter->GetComponent<dae::TextComponent>()->SetColor(252, 226, 5);
	pFpsCounter->AddComponent<dae::FPSCounterComponent>();
	scene.Add(pFpsCounter);
}

int main(int, char* []) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
	return 0;
};