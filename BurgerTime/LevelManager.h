#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H

#include <Singleton.h>
#include <GameObject.h>

#include <document.h>


class LevelManager final : real::Singleton<LevelManager>
{
public:
	using GameObjectPtrs = std::vector<std::shared_ptr<real::GameObject>>;


	real::GameObject* ParseLevel(real::Scene&, const std::string& filePath);

	GameObjectPtrs GetPlayerPtrs();
	GameObjectPtrs GetPlatformPtrs() const { return m_FloorPtrs; }
	GameObjectPtrs GetFloorPtrs() const { return m_PlatformPtrs; }
	GameObjectPtrs GetIngredientPtrs() const { return m_IngredientPtrs; }
private:
	friend class Singleton<LevelManager>;
	LevelManager() = default;

	std::vector<std::shared_ptr<real::GameObject>> m_PlayerPtrs;
	std::vector<std::shared_ptr<real::GameObject>> m_FloorPtrs;
	std::vector<std::shared_ptr<real::GameObject>> m_PlatformPtrs;
	std::vector<std::shared_ptr<real::GameObject>> m_IngredientPtrs;
	//std::vector<std::unique_ptr<real::GameObject>> m_FloorPtrs;

	void ParseIngredient(real::GameObject* pGameObject, const rapidjson::Document& document, const std::string& part);
};

#endif // LEVELMANAGER_H