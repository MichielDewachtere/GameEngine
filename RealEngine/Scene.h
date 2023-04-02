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

		void Add(std::unique_ptr<GameObject> object);
		void Remove(std::unique_ptr<GameObject> object);
		void RemoveAll();

		void Update();
		void Render() const;

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private: 
		explicit Scene(const std::string& name);

		std::string m_name;
		std::vector < std::unique_ptr<GameObject>> m_objects{};

		static unsigned int m_idCounter; 
	};

}

#endif // SCENE_H