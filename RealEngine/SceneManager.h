#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <string>
#include <vector>
#include <memory>

#include "Singleton.h"

namespace real
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& AddScene(Scene* scene);
		Scene& SetSceneActive(const std::string& name);

		Scene& GetActiveScene() const { return *m_pActiveScene; }

		void Update();
		void Render();
		void PostUpdate();
	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::shared_ptr<Scene>> m_ScenePtrs;
		std::shared_ptr<Scene> m_pActiveScene;
	};
}

#endif // SCENEMANAGER_H