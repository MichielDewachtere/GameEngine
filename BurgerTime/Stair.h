#ifndef STAIR_H
#define STAIR_H

#include <glm/vec2.hpp>

#include <GameObject.h>

class Stair
{
public:
	static real::GameObject* CreateStair(real::Scene* pScene, const glm::vec2 pos, float height, bool drawDebug = false, bool isHidden = false);
	static real::GameObject* CreateStair(real::GameObject* pGameObject, const glm::vec2 pos, float height, bool drawDebug = false, bool isHidden = false);

private:
	static void InitComponentsHidden(real::GameObject* pOwner, const glm::vec2 pos, float height, bool drawDebug);
	static void InitComponentsNormal(real::GameObject* pOwner, const glm::vec2 pos, float height, bool drawDebug);
};

#endif // STAIR_H