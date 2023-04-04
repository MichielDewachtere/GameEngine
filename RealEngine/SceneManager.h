#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "Singleton.h"

namespace real
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name);
		Scene& SetSceneActive(const std::string& name);

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