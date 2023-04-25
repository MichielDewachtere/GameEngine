#include "stdafx.h"
#include "LoadNextSceneCommand.h"

#include "Input.h"
#include "SceneManager.h"
#include "Scene.h"

LoadNextSceneCommand::LoadNextSceneCommand(real::GameObject* object, std::string name)
	: Command(object)
	, m_Name(std::move(name))
{
}

void LoadNextSceneCommand::Execute()
{
	real::SceneManager::GetInstance().SetSceneActive(m_Name);
	real::Input::GetInstance().SetInputMapActive(real::SceneManager::GetInstance().GetActiveScene().GetDefaultInputMap());
}
