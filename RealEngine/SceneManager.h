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
		Scene& CreateScene(const std::string& name, const std::string inputMapName = "empty");
		Scene& SetSceneActive(const std::string& name);

		Scene& GetActiveScene() const { return *m_pActiveScene; }

		void Update();
		void Render();
	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::shared_ptr<Scene>> m_ScenePtrs;
		std::shared_ptr<Scene> m_pActiveScene;
	};
}

#endif // SCENEMANAGER_H