#ifndef LEVELPARSER_H
#define LEVELPARSER_H

#include <document.h>
#include <Scene.h>

class LevelParser final
{
public:
	static real::GameObject* ParseLevel(real::Scene&, const std::string& filePath);

private:
	static void ParseIngredient(real::GameObject* pGameObject, const rapidjson::Document& document, const std::string& part);
};

#endif // LEVELPARSER_H