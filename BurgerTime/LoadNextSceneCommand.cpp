//#include "stdafx.h"
#include "LoadNextSceneCommand.h"

#include <Locator.h>

#include <Input.h>
#include <SceneManager.h>
#include <Scene.h>

#include "GameInfo.h"
#include "Logger.h"
#include "PlayerManager.h"

LoadNextSceneCommand::LoadNextSceneCommand(real::GameObject* object, std::string name)
	: Command(object)
	, m_Name(std::move(name))
{
}

void LoadNextSceneCommand::Execute()
{
	if (PlayerManager::GetInstance().GetAmountOfPlayers() == 0)
	{
		real::Logger::LogError("LoadNextSceneCommand => No player found, can not start the game");
		return;
	}

	real::SceneManager::GetInstance().SetSceneActive(m_Name);
	real::Input::GetInstance().SetInputMapActive(real::SceneManager::GetInstance().GetActiveScene().GetDefaultInputMap());

	if (m_Name.find("Level") != std::string::npos)
		real::Locator::GetAudioSystem().Play(Sounds::background);
}
