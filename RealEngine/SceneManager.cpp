 //#include "stdafx.h"
#include "SceneManager.h"

#include "GameTime.h"
#include "Input.h"
#include "Locator.h"
#include "Scene.h"

void real::SceneManager::Update()
{
	if (m_pActiveScene)
		m_pActiveScene->Update();

	if (m_LoadWithTimer)
	{
		m_LoadCountDown -= Time::GetInstance().GetElapsed();
		if (m_LoadCountDown <= 0)
		{
			m_LoadWithTimer = false;
			m_LoadCountDown = 0;

			SetSceneActive(m_SceneToLoad.get());
		}
	}
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

real::Scene& real::SceneManager::SetSceneActive(const std::string& name, float timer)
{
	Logger::LogInfo({"Scene {} is loaded in {} seconds"}, name, timer);

	timer = std::max(timer, 0.f);

	if (timer > 0 + FLT_EPSILON)
	{
		m_LoadWithTimer = true;
		m_LoadCountDown = timer;

		for (const auto& pScene : m_ScenePtrs)
		{
			if (pScene->GetName() == name)
			{
				m_SceneToLoad = pScene;
				return *pScene;
			}
		}

		return *m_pActiveScene;
	}

	for (const auto& pScene : m_ScenePtrs)
	{
		if (pScene->GetName() == name)
		{
			//return SetSceneActive(pScene.get(), timer);

			if (pScene->IsLoaded() == false)
				pScene->Load();

			if (m_pActiveScene != nullptr)
			{
				onSceneExit.Notify(*m_pActiveScene);
				onSceneSwitch.Notify(*m_pActiveScene, SceneSwitchState::exit);

				m_pActiveScene->RemoveAll();
				Locator::GetAudioSystem().StopAllSounds();
			}

			m_pActiveScene = pScene;

			pScene->Start();

			if (m_pActiveScene->GetDefaultInputMap() != "empty")
				Input::GetInstance().SetInputMapActive(m_pActiveScene->GetDefaultInputMap());

			Input::GetInstance().ReloadCommands();

			onSceneLoaded.Notify(*m_pActiveScene);
			onSceneSwitch.Notify(*m_pActiveScene, SceneSwitchState::loaded);


			return *pScene;
		}
	}

	throw std::runtime_error("Could not find a scene with the name \"" + name + "\". Please ensure that the name is spelled correctly and that the scene exists.");
}

real::Scene& real::SceneManager::SetSceneActive(real::Scene* scene, float timer)
{
	return SetSceneActive(scene->GetName(), timer);

	//timer = std::max(timer, 0.f);

	//if (timer > 0 + FLT_EPSILON)
	//{
	//	m_LoadWithTimer = true;
	//	m_LoadCountDown = timer;

	//	m_SceneToLoad = std::shared_ptr<Scene>(scene);

	//	return *m_SceneToLoad;
	//}

	//if (scene->IsLoaded() == false)
	//	scene->Load();

	//if (m_pActiveScene != nullptr)
	//{
	//	onSceneExit.Notify(*m_pActiveScene);
	//	m_pActiveScene->RemoveAll();
	//	Locator::GetAudioSystem().StopAllSounds();
	//}

	//m_pActiveScene = std::shared_ptr<Scene>(scene);
	////pScene->Start();

	//onSceneLoaded.Notify(*m_pActiveScene);

	//Input::GetInstance().ReloadCommands();

	//return *scene;
}
