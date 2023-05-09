#ifndef FLOOR_H
#define FLOOR_H

#include <GameObject.h>

class Floor
{
public:
	static real::GameObject* CreateFloor(real::Scene* pScene, const glm::vec2 pos, float height, bool drawDebug);
	static real::GameObject* CreateFloor(real::GameObject* pGameObject, const glm::vec2 pos, float height, bool drawDebug);

private:
	static void InitComponents(real::GameObject* pOwner, const glm::vec2 pos, float height, bool drawDebug);
};

#endif // FLOOR_H