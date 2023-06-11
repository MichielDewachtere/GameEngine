#ifndef GAMEINFO_H
#define GAMEINFO_H

#include <glm/vec4.hpp>

#include <string>

#include <Audio.h>
#include <SDL_mixer.h>

struct Tags
{
	inline static const std::string empty = "Empty";
	inline static const std::string player = "Player";
	inline static const std::string stair = "Stair";
	inline static const std::string hidden_stair = "HiddenStair";
	inline static const std::string floor = "Floor";
	inline static const std::string boundary = "Boundary";
	inline static const std::string hot_dog = "HotDog";
	inline static const std::string egg = "Egg";
	inline static const std::string pickle = "Pickle";
	inline static const std::string ingredient = "Ingredient";
	inline static const std::string plate = "Plate";
	inline static const std::string pepper = "Pepper";
	inline static const std::string player_spawn = "PlayerSpawn";
	inline static const std::string item_spawn = "ItemSpawn";
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

struct Scenes
{
	inline static const std::string main_menu = "MainMenu";
	inline static const std::string level01 = "Level01";
	inline static const std::string level02 = "Level02";
	inline static const std::string level03 = "Level03";
	inline static const std::string game_over_menu = "GameOverMenu";
};

struct InputMaps
{
	inline static const std::string gameplay = "Gameplay";
	inline static const std::string menu = "Menu";
};

struct Sounds
{
	inline static const real::Sound background	= { 1, MIX_MAX_VOLUME, -1, 0,
		"../data/audio/11_main.ogg" };
	inline static const real::Sound burger_falling = { 2, MIX_MAX_VOLUME, 1, -1,
		"../data/audio/0_burger_going_down.ogg" };
	inline static const real::Sound burger_landing = { 3, MIX_MAX_VOLUME, 1, -1,
		"../data/audio/1_burger_touching_floor.ogg" };
	inline static const real::Sound player_death = { 4, MIX_MAX_VOLUME, 1, 0,
		"../data/audio/2_die.ogg" };
	inline static const real::Sound high_score = { 5, MIX_MAX_VOLUME, 0, -1,
		"../data/audio/3_high_scores.ogg" };
	inline static const real::Sound life_added = { 6, MIX_MAX_VOLUME, 0, -1,
		"../data/audio/6_one_up.ogg" };
	inline static const real::Sound enemy_crushed = { 7, MIX_MAX_VOLUME, 0, -1,
		"../data/audio/7_enemy_crushed.ogg" };
	inline static const real::Sound enemy_fall = { 8, MIX_MAX_VOLUME, 0, -1,
		"../data/audio/8_enemy_fall.ogg" };
	inline static const real::Sound pepper_added = { 9, MIX_MAX_VOLUME, 0, -1,
		"../data/audio/9_pepper_up.ogg" };
	inline static const real::Sound pepper_thrown = { 10, MIX_MAX_VOLUME, 0, -1,
		"../data/audio/14_pepper.ogg" };
	inline static const real::Sound enemy_stunned = { 11, MIX_MAX_VOLUME, 0, -1,
		"../data/audio/15_peppered.ogg" };
	inline static const real::Sound burger_part_activated = { 12, MIX_MAX_VOLUME, 1, -1,
		"../data/audio/16_stepping_on_burger.ogg" };
	inline static const real::Sound level_completed = { 13, MIX_MAX_VOLUME, 0, 0,
		"../data/audio/17_win.ogg" };
	inline static const real::Sound level_intro = { 14, MIX_MAX_VOLUME, 0, 0,
		"../data/audio/10_level_intro.ogg" };
	inline static const real::Sound menu_select = { 15, MIX_MAX_VOLUME, 0, -1,
		"../data/audio/18_menu_select.ogg" };
	inline static const real::Sound menu_background = { 16, MIX_MAX_VOLUME, -1, 0,
		"../data/audio/19_burgermenu.ogg" };
};

#endif // GAMEINFO_H