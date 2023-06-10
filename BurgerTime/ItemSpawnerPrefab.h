#ifndef ITEMSPAWNERPREFAB_H
#define ITEMSPAWNERPREFAB_H

#include <glm/vec2.hpp>

#include <GameObject.h>

class ItemSpawnerPrefab final
{
public:
	static real::GameObject* CreateItemSpawnerPrefab(real::Scene* pScene, const glm::vec2 pos);
	static real::GameObject* CreateItemSpawnerPrefab(real::GameObject* pGameObject, const glm::vec2 pos);

private:
	static void InitComponents(real::GameObject* pOwner, const glm::vec2 pos);
};

#endif // ITEMSPAWNERPREFAB_H