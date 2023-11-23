#include "Prefab.h"

#include "GameObject.h"
#include "Scene.h"

real::Prefab::Prefab(Scene& scene)
	: m_pGameObject(scene.CreateGameObject())
{
}

real::Prefab::Prefab(GameObject& gameObject)
	: m_pGameObject(gameObject.CreateGameObject())
{
}
