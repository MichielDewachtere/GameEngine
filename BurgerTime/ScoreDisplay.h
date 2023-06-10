#ifndef SCOREDISPLAY_H
#define SCOREDISPLAY_H

#include <Component.h>
#include <Observer.h>

class ScoreDisplay final : public real::Component,
	public real::Observer<int>,
	public real::Observer<real::Scene&>
{
public:
	explicit ScoreDisplay(real::GameObject* pOwner);
	virtual ~ScoreDisplay() override;

	void Start() override;
	void Update() override {};
	bool CanRender() const override { return false; }

	virtual void HandleEvent(int) override;
	void HandleEvent(real::Scene&) override;
	virtual void OnSubjectDestroy() override {}
private:
	int m_Score{ 0 };

	void UpdateScoreText(int);
};

#endif // SCOREDISPLAY_H