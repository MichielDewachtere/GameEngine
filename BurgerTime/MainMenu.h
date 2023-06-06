#ifndef MAINMENU_H
#define MAINMENU_H

#include <Scene.h>

class MainMenu final : public real::Scene
{
public:
	explicit MainMenu();
	virtual ~MainMenu() override = default;
	MainMenu(const MainMenu& other) = delete;
	MainMenu(MainMenu&& other) = delete;
	MainMenu& operator=(const MainMenu& other) = delete;
	MainMenu& operator=(MainMenu&& other) = delete;

	void Load() override;
};

#endif // MAINMENU_H