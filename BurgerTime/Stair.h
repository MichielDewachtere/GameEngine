#ifndef STAIR_H
#define STAIR_H

#include <GameObject.h>

class Stair
{
public:
	static real::GameObject* CreateStair(real::Scene* pScene, const glm::vec2 pos, float height, bool drawDebug);
	static real::GameObject* CreateStair(real::GameObject* pGameObject, const glm::vec2 pos, float height, bool drawDebug);

private:
	static void InitComponents(real::GameObject* pOwner, const glm::vec2 pos, float height, bool drawDebug);
};

#endif // STAIR_H