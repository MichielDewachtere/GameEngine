#ifndef LEVELPARSER_H
#define LEVELPARSER_H

#include <Scene.h>

class LevelParser final
{
public:
	static real::GameObject* ParseLevel(real::Scene&, const std::string& filePath);
};

#endif // LEVELPARSER_H