#ifndef MAINMENU_H
#define MAINMENU_H

#include <Scene.h>
#include <Minigin.h>

class MainMenu final : public real::Scene
{
public:
	explicit MainMenu(real::WindowSettings settings);
	virtual ~MainMenu() override = default;
	MainMenu(const MainMenu& other) = delete;
	MainMenu(MainMenu&& other) = delete;
	MainMenu& operator=(const MainMenu& other) = delete;
	MainMenu& operator=(MainMenu&& other) = delete;

	void Load() override;

private:
	real::WindowSettings m_Settings;
};

#endif // MAINMENU_H