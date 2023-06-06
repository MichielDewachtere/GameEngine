//#include "stdafx.h"
#include "Scene.h"

#include "TransformComponent.h"

using namespace real;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(std::string name, std::string inputMapName)
	: m_Name(std::move(name))
	, m_InputMapName(std::move(inputMapName))
{
}

Scene::~Scene() = default;

GameObject* Scene::CreateGameObject()
{
	auto pGameObject{ std::make_unique<GameObject>(this) };
	//pGameObject->Init();
	pGameObject->AddComponent<TransformComponent>();

	GameObject* pGameObjectPtr{ pGameObject.get() };

	Add(std::move(pGameObject));
	return pGameObjectPtr;
}

void Scene::Add(std::shared_ptr<GameObject> object)
{
	m_objects.emplace_back(object);
}

void Scene::Remove(std::shared_ptr<GameObject> object)
{
	std::erase(m_objects, object);
}

void Scene::RemoveAll()
{
	//m_objects.clear();
	for (const auto& gameObject : m_objects)
	{
		gameObject->Destroy();
	}

	m_objects.clear();
}

std::vector<GameObject*> Scene::FindObjectsWithTag(const std::string& tag) const
{
	std::vector<GameObject*> goPtrs;

	for (const auto& gameObject : m_objects)
	{
		//if (gameObject->GetTag() == tag)
		//	goPtrs.push_back(gameObject.get());

		for (const auto go : gameObject->GetObjectsWithTag(tag))
		{
			goPtrs.push_back(go);
		}
	}

	return goPtrs;
}

GameObject* Scene::FindObject(gameobject_id id) const
{
	for (const auto& gameObject : m_objects)
	{
		const auto go = gameObject->GetObject(id);

		if (go != nullptr)
			return go;
	}

	return nullptr;
}

void Scene::Start()
{
	for (const auto& object : m_objects)
	{
		object->Start();
	}
}

void Scene::Update()
{
	for(const auto& object : m_objects)
	{
		object->Update();
	}
}

void Scene::Render() const
{
	for (const auto& object : m_objects)
	{
		object->Render();

		if (m_DebugRender)
			object->DebugRender();
	}
}

void Scene::PostUpdate()
{
	for (const auto& object : m_objects)
		object->PostUpdate();
}

