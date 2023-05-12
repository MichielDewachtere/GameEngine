#ifndef PLATE_H
#define PLATE_H

#include <GameObject.h>

#include "HealthComponent.h"
#include "HealthComponent.h"

class Plate
{
public:
	static real::GameObject* CreatePlate(real::Scene* pScene, const glm::vec2 pos, bool drawDebug);
	static real::GameObject* CreatePlate(real::GameObject* pGameObject, const glm::vec2 pos, bool drawDebug);

private:
	static void InitComponents(real::GameObject* pOwner, const glm::vec2 pos, bool drawDebug);
};

#endif // PLATE_H