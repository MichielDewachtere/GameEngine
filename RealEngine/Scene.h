#ifndef SCENE_H
#define SCENE_H

#include "SceneManager.h"
#include "GameObject.h"

namespace real
{
	//class SceneManager;

	//class GameObject;
	//using gameobject_id = unsigned short;

	class Scene
	{
	public:
		virtual ~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		GameObject* CreateGameObject();

		//template <class T>
		//T* CreateGameObject();

		void Add(const std::shared_ptr<GameObject>& object);
		//void Add(GameObject* object);
		void Remove(const std::shared_ptr<GameObject>& object);
		//void Remove(GameObject* object);
		void RemoveAll();
		std::vector<GameObject*> FindObjectsWithTag(const std::string& tag) const;
		GameObject* FindObject(gameobject_id id) const;

		void Start();
		void Update();
		void Render() const;
		void PostUpdate();

		std::string GetName() const { return m_Name; }

		void SetDefaultInputMap(std::string name) { m_InputMapName = std::move(name); }
		std::string GetDefaultInputMap() const { return m_InputMapName; }

		void SetDebugRendering(bool enable) { m_DebugRender = enable; }

		bool IsLoaded() { return m_IsLoaded; }
		virtual void Load() = 0;


	protected: 
		explicit Scene(std::string name, std::string inputMapName = "empty");
		friend Scene& SceneManager::AddScene(Scene* scene);

		std::string m_Name;
		std::string m_InputMapName;
		std::vector < std::shared_ptr<GameObject>> m_ObjectPtrs{};
		//std::vector<std::unique_ptr<GameObject>> m_ObjectPtrs{};

		static unsigned int m_IdCounter;

		bool m_DebugRender{};
		bool m_IsLoaded{};
	};

	//template <class T>
	//T* Scene::CreateGameObject()
	//{
	//	const auto pGameObject = std::make_unique<T>(this);
	//	pGameObject->Init();

	//	//m_ObjectPtrs.emplace_back(std::move(pGameObject));
	//	m_ObjectPtrs.push_back(pGameObject);
	//	return pGameObject.get();
	//}
}

#endif // SCENE_H