
#include "stdafx.h"
#include "LevelParser.h"

#include <fstream>

#include <TextureComponent.h>
#include <ResourceManager.h>
#include <TransformComponent.h>
#include <ColliderComponent.h>

#include "Floor.h"
#include "GameInfo.h"
#include "IngredientPrefab.h"
#include "Plate.h"
#include "Stair.h"

real::GameObject* LevelParser::ParseLevel(real::Scene& pScene, const std::string& file)
{
    std::string fileName = "../Data/Levels/" + file;
    std::ifstream inputFile(fileName);

    // Check if file was opened successfully
    if (!inputFile.is_open()) {
        // Handle error opening file
        std::cout << "File didn't open\n";
    }

    // Read the entire file into a string
    std::string fileContents((std::istreambuf_iterator<char>(inputFile)),
        std::istreambuf_iterator<char>());

    // Close the input file
    inputFile.close();

    rapidjson::Document document;
    document.Parse(fileContents.c_str());

    const rapidjson::Value& value = document["levelTexturePath"];
    const auto pBackGroundTexture = real::ResourceManager::GetInstance().LoadTexture(value.GetString());

    const auto pLevel = pScene.CreateGameObject();
    //pLevel->GetComponent<real::TransformComponent>()->Translate(48, 32);
    pLevel->AddComponent<real::TextureComponent>()->SetTexture(pBackGroundTexture);

    const auto pLevelBoundaries = pLevel->CreateGameObject();
    pLevelBoundaries->SetTag(Tags::boundary);
    pLevelBoundaries->GetComponent<real::TransformComponent>()->SetLocalPosition(0, -9);
    glm::vec2 levelBoundaries = pBackGroundTexture->GetSize();
    pLevelBoundaries->AddComponent<real::ColliderComponent>(levelBoundaries)->EnableDebugRendering(false, Colors::red);

    // FLOORS
    for (const auto& floor : document["floors"].GetArray())
        Floor::CreateFloor(pLevel, { floor[0].GetDouble(), floor[1].GetDouble() }, static_cast<float>(floor[2].GetDouble()), true);
    // STAIRS
    for (const auto& stair : document["stairs"].GetArray())
        Stair::CreateStair(pLevel, { stair[0].GetDouble(), stair[1].GetDouble() }, static_cast<float>(stair[2].GetDouble()), false);
    // BUN_TOP
    ParseIngredient(pLevel, document, "bun_top");
    // LETTUCE
    ParseIngredient(pLevel, document, "lettuce");
    // TOMATO
    // CHEESE
	// PATTY
    ParseIngredient(pLevel, document, "patty");
    // BUN_BOTTOM
    ParseIngredient(pLevel, document, "bun_bottom");
    //PLATES
    for (const auto& plate : document["plates"].GetArray())
        Plate::CreatePlate(pLevel, { plate[0].GetDouble(), plate[1].GetDouble() }, true);

    return pLevel;
}

void LevelParser::ParseIngredient(real::GameObject* pGameObject, const rapidjson::Document& document, const std::string& part)
{
    const rapidjson::Value& bunBottomValue = document[part.c_str()];
    const std::string bunBottomTexturePath = bunBottomValue["texturePath"].GetString();
    for (const auto& bunBottom : bunBottomValue["position"].GetArray())
        IngredientPrefab::CreateIngredient(pGameObject, bunBottomTexturePath, { bunBottom[0].GetDouble(), bunBottom[1].GetDouble() }, true);
}
