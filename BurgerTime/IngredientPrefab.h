#ifndef INGREDIENTPREFAB_H
#define INGREDIENTPREFAB_H

#include <GameObject.h>

class IngredientPrefab
{
public:
	static real::GameObject* CreateIngredient(real::Scene* pScene, const std::string& texturePath, const glm::vec2 pos, bool drawDebug);
	static real::GameObject* CreateIngredient(real::GameObject* pGameObject, const std::string&, const glm::vec2 pos, bool drawDebug);

private:
	static void InitComponents(real::GameObject* pOwner, const std::string&, const glm::vec2 pos, bool drawDebug);
};

#endif // INGREDIENTPREFAB_H