#ifndef SPAWNPOINT_H
#define SPAWNPOINT_H

#include <glm/vec2.hpp>

#include <GameObject.h>

class SpawnPoint final
{
public:
	static real::GameObject* CreateSpawnPoint(real::Scene* pScene, const glm::vec2 pos, const std::vector<std::string>& enemies);
	static real::GameObject* CreateSpawnPoint(real::GameObject* pGameObject, const glm::vec2 pos, const std::vector<std::string>& enemies);

private:
	static void InitComponents(real::GameObject* pOwner, const glm::vec2 pos, const std::vector<std::string>& enemies);
};

#endif // SPAWNPOINT_H