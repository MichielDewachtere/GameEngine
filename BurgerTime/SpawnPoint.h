#ifndef SPAWNOINT_H
#define SPAWNOINT_H

#include <GameObject.h>

class SpawnPoint final
{
public:
	static real::GameObject* CreateSpawnPoint(real::Scene* pScene, const glm::vec2 pos, const std::string& type);
	static real::GameObject* CreateSpawnPoint(real::GameObject* pGameObject, const glm::vec2 pos, const std::string& type);

private:
	static void InitComponents(real::GameObject* pOwner, const glm::vec2 pos, const std::string& type);
};

#endif // SPAWNOINT_H