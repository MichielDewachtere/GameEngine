#ifndef GAMEOVERMENU_H
#define GAMEOVERMENU_H

#include <Scene.h>
#include <Minigin.h>

class GameOverMenu final : public real::Scene
{
public:
	explicit GameOverMenu(real::WindowSettings settings);
	virtual ~GameOverMenu() override = default;
	GameOverMenu(const GameOverMenu& other) = delete;
	GameOverMenu(GameOverMenu&& other) = delete;
	GameOverMenu& operator=(const GameOverMenu& other) = delete;
	GameOverMenu& operator=(GameOverMenu&& other) = delete;

	void Load() override;

private:
	real::WindowSettings m_Settings;
};

#endif // GAMEOVERMENU_H