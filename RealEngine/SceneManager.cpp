#include "stdafx.h"
#include "SceneManager.h"

#include "Scene.h"

void real::SceneManager::Update()
{
	m_pActiveScene->Update();
}

void real::SceneManager::Render()
{
	m_pActiveScene->Render();
}

real::Scene& real::SceneManager::CreateScene(const std::string& name, const std::string inputMapName)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name, inputMapName));
	for (const auto& pScene : m_ScenePtrs)
	{
		if (pScene->GetName() == name)
			throw std::runtime_error("A scene with the name \"" + name + "\" already exists. Please choose a different name for your scene.");
	}

	m_ScenePtrs.push_back(scene);

	if (m_ScenePtrs.size() == 1)
		m_pActiveScene = scene;

	return *scene;
}

real::Scene& real::SceneManager::SetSceneActive(const std::string& name)
{
	for (const auto& pScene : m_ScenePtrs)
	{
		if (pScene->GetName() == name)
		{
			m_pActiveScene = pScene;
			return *pScene;
		}
	}

	throw std::runtime_error("Could not find a scene with the name \"" + name + "\". Please ensure that the name is spelled correctly and that the scene exists.");

	//return *m_pActiveScene;
}