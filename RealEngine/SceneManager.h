#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <string>
#include <vector>
#include <memory>

#include "Singleton.h"
#include "Subject.h"

namespace real
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& AddScene(Scene* scene);
		Scene& SetSceneActive(const std::string& name, float timer = 0);
		Scene& SetSceneActive(Scene* scene, float timer = 0);

		Scene& GetActiveScene() const { return *m_pActiveScene; }

		void Update();
		void Render();
		void PostUpdate();

		Subject<Scene&> onSceneExit{};
		Subject<Scene&> onSceneLoaded{};
	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::shared_ptr<Scene>> m_ScenePtrs;
		std::shared_ptr<Scene> m_pActiveScene{nullptr};

		bool m_LoadWithTimer{};
		float m_LoadCountDown{};
		//std::string m_SceneToLoad{ "empty" };
		std::shared_ptr<Scene> m_SceneToLoad{};
	};
}

#endif // SCENEMANAGER_H