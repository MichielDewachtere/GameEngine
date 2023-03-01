#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "GameObject.h"
#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "TextureComponent.h"
#include "TransformComponent.h"

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	const auto pBackgroundTexture = dae::ResourceManager::GetInstance().LoadTexture("background.tga");

	const auto pBackground = std::make_shared<dae::GameObject>();
	pBackground->AddComponent<dae::TransformComponent, std::shared_ptr<dae::GameObject>>(pBackground)->SetPosition(0, 0);
	pBackground->AddComponent<dae::TextureComponent, std::shared_ptr<dae::GameObject>>(pBackground)->SetTexture(pBackgroundTexture);
	scene.Add(pBackground);

	//auto go = std::make_shared<dae::GameObject>();
	//go->SetTexture("background.tga");
	//scene.Add(go);

	//go = std::make_shared<dae::GameObject>();
	//go->SetTexture("logo.tga");
	//go->SetPosition(216, 180);
	//scene.Add(go);

	//auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	//auto to = std::make_shared<dae::TextComponent>("Programming 4 Assignment", font);
	//to->SetPosition(80, 20);
	//scene.Add(to);
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}