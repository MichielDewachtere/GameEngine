#ifndef SCOREDISPLAY_H
#define SCOREDISPLAY_H

#include <Component.h>
#include <Observer.h>
#include <Subject.h>
#include <SceneManager.h>


class ScoreDisplay final :
	public real::Component,
	public real::Observer<int>,
	public real::Observer<real::Scene&, real::SceneManager::SceneSwitchState>
{
public:
	explicit ScoreDisplay(real::GameObject* pOwner);
	virtual ~ScoreDisplay() override;
	ScoreDisplay(const ScoreDisplay& other) = delete;
	ScoreDisplay operator=(const ScoreDisplay& rhs) = delete;
	ScoreDisplay(ScoreDisplay&& other) = delete;
	ScoreDisplay operator=(ScoreDisplay&& rhs) = delete;

	void Start() override;
	void Update() override {}
	bool CanRender() const override { return false; }

	virtual void HandleEvent(int) override;
	void HandleEvent(real::Scene&, real::SceneManager::SceneSwitchState) override;
	virtual void OnSubjectDestroy() override {}

	real::Subject<int> scoreChanged{};
	real::Subject<> AddLife{};

private:
	int m_Score{ 0 };
	int m_BonusLivesAdded{};

	void UpdateScoreText(int);
};

#endif // SCOREDISPLAY_H