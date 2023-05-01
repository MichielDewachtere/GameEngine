#ifndef SCENE_H
#define SCENE_H

#include "SceneManager.h"
#include "GameObject.h"

namespace real
{
	//class GameObject;
	class Scene final
	{
		friend Scene& SceneManager::CreateScene(const std::string& name, std::string inputMapName);
	public:
		GameObject* CreateGameObject();

		template <class T>
		T* CreateGameObject();

		void Add(std::unique_ptr<GameObject> object);
		void Remove(std::unique_ptr<GameObject> object);
		void RemoveAll();
		std::vector<GameObject*> FindObjectsWithTag(const std::string& tag) const;

		void Update();
		void Render() const;

		std::string GetName() const { return m_Name; }

		void SetDefaultInputMap(std::string name) { m_InputMapName = std::move(name); }
		std::string GetDefaultInputMap() const { return m_InputMapName; }

		void SetDebugRendering(bool enable) { m_DebugRender = enable; }

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private: 
		explicit Scene(std::string name, std::string inputMapName = "empty");

		std::string m_Name;
		std::string m_InputMapName;
		std::vector < std::unique_ptr<GameObject>> m_objects{};

		static unsigned int m_idCounter;

		bool m_DebugRender{};
	};

	template <class T>
	T* Scene::CreateGameObject()
	{
		auto pGameObject = std::make_unique<T>(this);
		pGameObject->Init();
	
		T* pGameObjectPtr = pGameObject.get();
	
		m_objects.emplace_back(std::move(pGameObject));
		return pGameObjectPtr;
	}
}

#endif // SCENE_H