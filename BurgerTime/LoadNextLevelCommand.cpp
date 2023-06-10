//#include "stdafx.h"
#include "LoadNextLevelCommand.h"

#include <Locator.h>

#include <Input.h>
#include <SceneManager.h>
#include <Scene.h>

#include "GameInfo.h"
#include "Logger.h"
#include "PlayerManager.h"

LoadNextLevelCommand::LoadNextLevelCommand(real::GameObject* object, std::string name)
	: Command(object)
	, m_Name(std::move(name))
{
}

void LoadNextLevelCommand::Execute()
{
	if (PlayerManager::GetInstance().GetAmountOfPlayers() == 0)
	{
		real::Logger::LogError("LoadNextLevelCommand => No player found, can not start the game");
		return;
	}

	const int nextLevelNumber = PlayerManager::GetInstance().GetCurrentLevel() + 1;
	if (nextLevelNumber <= 3)
	{
		real::SceneManager::GetInstance().SetSceneActive("Level0" + std::to_string(nextLevelNumber), 0);
		real::Input::GetInstance().SetInputMapActive(real::SceneManager::GetInstance().GetActiveScene().GetDefaultInputMap());
	}
}
