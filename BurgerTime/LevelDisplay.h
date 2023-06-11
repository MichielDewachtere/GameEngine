#ifndef LEVELDISPLAY_H
#define LEVELDISPLAY_H

#include <Component.h>
#include <Observer.h>
#include <SceneManager.h>
#include <SpriteComponent.h>

class LevelDisplay final : public real::Component
	, public real::Observer<>
	, public real::Observer<real::Scene&, real::SceneManager::SceneSwitchState>
{
public:
	explicit LevelDisplay(real::GameObject* pOwner);
	virtual ~LevelDisplay() override;
	LevelDisplay(const LevelDisplay& other) = delete;
	LevelDisplay operator=(const LevelDisplay& rhs) = delete;
	LevelDisplay(LevelDisplay&& other) = delete;
	LevelDisplay operator=(LevelDisplay&& rhs) = delete;

	void Update() override {}
	bool CanRender() const override { return false; }

	void HandleEvent() override;
	void HandleEvent(real::Scene&, real::SceneManager::SceneSwitchState) override;
	void OnSubjectDestroy() override {}

private:
	int m_LevelsCompleted{ 1 };
	real::SpriteSheet m_SpriteSheet{};

	real::GameObject* m_NormalIcons{};
	real::GameObject* m_FiveIcon{};
	real::GameObject* m_TenIcons{};

	void InitIcons();
};

#endif // LEVELDISPLAY_H