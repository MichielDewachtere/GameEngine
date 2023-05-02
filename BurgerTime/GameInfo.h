#ifndef GAMEINFO_H
#define GAMEINFO_H

#include <string>

struct Tags
{
	inline static const std::string player = "Player";
	inline static const std::string stair = "Stair";
	inline static const std::string floor = "Floor";
	inline static const std::string boundary = "Boundary";
	inline static const std::string hotDog = "HotDog";
	inline static const std::string egg = "Egg";
};

struct Colors
{
	inline static const glm::vec4 red = { 255,0,0,255 };
	inline static const glm::vec4 green = { 0,255,0,255 };
	inline static const glm::vec4 blue = { 0,0,255,255 };
	inline static const glm::vec4 white = { 255,255,255,255 };
	inline static const glm::vec4 black = { 0,0,0,255 };
	inline static const glm::vec4 yellow = { 255,255,0,255 };
	inline static const glm::vec4 cyan = { 0,255,255,255 };
	inline static const glm::vec4 purple = { 255,0,255,255 };
	inline static const glm::vec4 appelblauwzeegroen = { 0,237,116,255 };
};

#endif // GAMEINFO_H