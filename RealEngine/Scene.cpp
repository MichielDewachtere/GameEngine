//#include "stdafx.h"
#include "Scene.h"

#include "TransformComponent.h"
//#include "GameObject.h"

using namespace real;

unsigned int Scene::m_IdCounter = 0;

Scene::Scene(std::string name, std::string inputMapName)
	: m_Name(std::move(name))
	, m_InputMapName(std::move(inputMapName))
{
}

Scene::~Scene() = default;

GameObject* Scene::CreateGameObject()
{
	const auto pGameObject = std::make_shared<GameObject>(this);
	//const auto pGameObject = new GameObject(this);
	pGameObject->AddComponent<TransformComponent>();

	//GameObject* pGameObjectPtr{ pGameObject.get() };

	Add(pGameObject);
	//Add(std::move(pGameObject));
	return pGameObject.get();
}

void Scene::Add(const std::shared_ptr<GameObject>& object)
//void Scene::Add(GameObject* object)
{
	//m_ObjectPtrs.push_back(std::shared_ptr<GameObject>(object));
	m_ObjectPtrs.emplace_back(object);
}

void Scene::Remove(const std::shared_ptr<GameObject>& object)
//void Scene::Remove(GameObject* object)
{
	//std::erase(m_ObjectPtrs, std::unique_ptr<GameObject>(object));
	std::erase_if(m_ObjectPtrs,
	              [object](const std::shared_ptr<GameObject>& ptr) {
		              return ptr == object;
	              });
}

void Scene::RemoveAll()
{
	//m_ObjectPtrs.clear();
	for (const auto& gameObject : m_ObjectPtrs)
	{
		gameObject->Destroy();
	}

	m_IsLoaded = false;
	m_ObjectPtrs.clear();
}

std::vector<GameObject*> Scene::FindObjectsWithTag(const std::string& tag) const
{
	std::vector<GameObject*> goPtrs;

	for (const auto& gameObject : m_ObjectPtrs)
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
	for (const auto& gameObject : m_ObjectPtrs)
	{
		const auto go = gameObject->GetObject(id);

		if (go != nullptr)
			return go;
	}

	return nullptr;
}

void Scene::Start()
{
	for (const auto& object : m_ObjectPtrs)
	{
		object->Start();
	}
}

void Scene::Update()
{
	for(const auto& object : m_ObjectPtrs)
	{
		object->Update();
	}
}

void Scene::Render() const
{
	for (const auto& object : m_ObjectPtrs)
	{
		object->Render();

		if (m_DebugRender)
			object->DebugRender();
	}
}

void Scene::PostUpdate()
{
	for (const auto& object : m_ObjectPtrs)
		object->PostUpdate();
}

