//#include "stdafx.h"
#include "SceneManager.h"

#include "Input.h"
#include "Scene.h"

void real::SceneManager::Update()
{
	if (m_pActiveScene)
		m_pActiveScene->Update();
}

void real::SceneManager::Render()
{
	if (m_pActiveScene)
		m_pActiveScene->Render();
}

void real::SceneManager::PostUpdate()
{
	if (m_pActiveScene)
		m_pActiveScene->PostUpdate();
}

real::Scene& real::SceneManager::AddScene(real::Scene* scene)
{
	const auto& newScene = std::shared_ptr<Scene>(scene);
	for (const auto& pScene : m_ScenePtrs)
	{
		if (pScene->GetName() == newScene->GetName())
			throw std::runtime_error("A scene with the name \"" + scene->GetName() + "\" already exists. Please choose a different name for your scene.");
	}

	m_ScenePtrs.push_back(newScene);

	return *newScene;
}

real::Scene& real::SceneManager::SetSceneActive(const std::string& name)
{
	for (const auto& pScene : m_ScenePtrs)
	{
		if (pScene->GetName() == name)
		{
			//m_pActiveScene = pScene;
  			//pScene->Start();
			//return *pScene;

			if (pScene->IsLoaded() == false)
				pScene->Load();

			if (m_pActiveScene != nullptr)
				m_pActiveScene->RemoveAll();

			m_pActiveScene = pScene;
			//pScene->Start();

			onSceneLoaded.Notify(*m_pActiveScene);

			Input::GetInstance().ReloadCommands();

			return *pScene;
		}
	}
	

	throw std::runtime_error("Could not find a scene with the name \"" + name + "\". Please ensure that the name is spelled correctly and that the scene exists.");

	//return *m_pActiveScene;
}