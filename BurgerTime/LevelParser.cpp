
#include "stdafx.h"
#include "LevelParser.h"

#include <fstream>

#include <document.h>

#include <TextureComponent.h>
#include <ResourceManager.h>
#include <TransformComponent.h>
#include <ColliderComponent.h>

#include "Floor.h"
#include "GameInfo.h"
#include "IngredientPrefab.h"
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
    pLevel->GetComponent<real::TransformComponent>()->Translate(48, 32);
    pLevel->AddComponent<real::TextureComponent>()->SetTexture(pBackGroundTexture);

    const auto pLevelBoundaries = pLevel->CreateGameObject();
    pLevelBoundaries->SetTag(Tags::boundary);
    pLevelBoundaries->GetComponent<real::TransformComponent>()->SetLocalPosition(0, -9);
    glm::vec2 levelBoundaries = pBackGroundTexture->GetSize();
    pLevelBoundaries->AddComponent<real::ColliderComponent>(levelBoundaries)->EnableDebugRendering(false, Colors::red);

    // FLOORS
    for (const auto& floor : document["floors"].GetArray())
        Floor::CreateFloor(pLevel, { floor[0].GetDouble(), floor[1].GetDouble() }, static_cast<float>(floor[2].GetDouble()), false);

    // STAIRS
    for (const auto& stair : document["stairs"].GetArray())
        Stair::CreateStair(pLevel, { stair[0].GetDouble(), stair[1].GetDouble() }, static_cast<float>(stair[2].GetDouble()), false);

    // BUN_TOP
    const rapidjson::Value& bunTop = document["bun_top"];
    const std::string bunTopTexturePath = bunTop["texturePath"].GetString();
    for (const auto& top : bunTop["position"].GetArray())
        IngredientPrefab::CreateIngredient(pLevel, bunTopTexturePath, { top[0].GetDouble(), top[1].GetDouble() }, false);

    // LETTUCE
    const rapidjson::Value& lettuceValue = document["lettuce"];
    const std::string lettuceTexturePath = lettuceValue["texturePath"].GetString();
    for (const auto& lettuce : lettuceValue["position"].GetArray())
        IngredientPrefab::CreateIngredient(pLevel, lettuceTexturePath, { lettuce[0].GetDouble(), lettuce[1].GetDouble() }, false);

    // TOMATO

    // CHEESE

	// PATTY
    const rapidjson::Value& pattyValue = document["patty"];
    const std::string pattyTexturePath = pattyValue["texturePath"].GetString();
    for (const auto& patty : pattyValue["position"].GetArray())
        IngredientPrefab::CreateIngredient(pLevel, pattyTexturePath, { patty[0].GetDouble(), patty[1].GetDouble() }, false);

	// BUN_BOTTOM
	const rapidjson::Value& bunBottomValue = document["bun_bottom"];
    const std::string bunBottomTexturePath = bunBottomValue["texturePath"].GetString();
    for (const auto& bunBottom : bunBottomValue["position"].GetArray())
        IngredientPrefab::CreateIngredient(pLevel, bunBottomTexturePath, { bunBottom[0].GetDouble(), bunBottom[1].GetDouble() }, false);

    return pLevel;
}
