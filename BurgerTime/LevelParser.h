#ifndef LEVELPARSER_H
#define LEVELPARSER_H

// Temporarely dissable the following warning as the RapidJSON library has trouble compiling from cpp17 onwards.
#pragma warning(disable : 4996)
#include <document.h>
#pragma warning(default : 4996)
#include <Scene.h>

class LevelParser final
{
public:
	static real::GameObject* ParseLevel(real::Scene&, const std::string& filePath);

private:
	static void ParseIngredient(real::GameObject* pGameObject, const rapidjson::Document& document, const std::string& part);
};

#endif // LEVELPARSER_H