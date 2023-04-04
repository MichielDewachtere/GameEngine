#ifndef SCENE_H
#define SCENE_H

#include "SceneManager.h"
#include "GameObject.h"

namespace real
{
	//class GameObject;
	class Scene final
	{
		friend Scene& SceneManager::CreateScene(const std::string& name);
	public:
		GameObject* CreateGameObject();

		template <class T>
		T* CreateGameObject();

		void Add(std::unique_ptr<GameObject> object);
		void Remove(std::unique_ptr<GameObject> object);
		void RemoveAll();

		void Update();
		void Render() const;

		std::string GetName() const { return m_Name; }

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private: 
		explicit Scene(const std::string& name);

		std::string m_Name;
		std::vector < std::unique_ptr<GameObject>> m_objects{};

		static unsigned int m_idCounter; 
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