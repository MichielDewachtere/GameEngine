//#include "stdafx.h"

#include <Minigin.h> 

#include <InputMap.h>
#include <SceneManager.h>
#include <Scene.h>

#ifdef USE_STEAM
#include <Achievements.h>
#endif 

#include "MainMenu.h"
#include "Level01.h"
#include "Level02.h"
#include "GameInfo.h"
#include "PlayerManager.h"

real::WindowSettings g_window;

void load()
{
	PlayerManager::GetInstance().Init();

	real::SceneManager::GetInstance().AddScene(new MainMenu());
	real::SceneManager::GetInstance().AddScene(new Level01());
	real::SceneManager::GetInstance().AddScene(new Level02());
	//real::SceneManager::GetInstance().AddScene(new Level03());

	real::SceneManager::GetInstance().SetSceneActive(Scenes::main_menu);
	real::Input::GetInstance().SetInputMapActive(InputMaps::menu);

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
	g_window.height = 710;

	real::Minigin engine("../Data/", g_window);
	engine.Run(load);

#ifdef USE_STEAM
	SteamAPI_Shutdown();
#endif // USE_STEAM
	return 0;
};