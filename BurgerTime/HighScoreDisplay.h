#ifndef HIGHSCOREDISPLAY_H
#define HIGHSCOREDISPLAY_H

#include <Component.h>
#include <Observer.h>

#include "ScoreDisplay.h"

class HighScoreDisplay final : public real::Component
	, public real::Observer<int>
{
public:
	explicit HighScoreDisplay(real::GameObject* pOwner, ScoreDisplay* pScoreDisplay);
	virtual ~HighScoreDisplay() override;
	HighScoreDisplay(const HighScoreDisplay& other) = delete;
	HighScoreDisplay operator=(const HighScoreDisplay& rhs) = delete;
	HighScoreDisplay(HighScoreDisplay&& other) = delete;
	HighScoreDisplay operator=(HighScoreDisplay&& rhs) = delete;
	
	virtual void Update() override {}
	virtual bool CanRender() const override { return false; }

	void HandleEvent(int) override;
	void OnSubjectDestroy() override {}
private:
	ScoreDisplay* m_ScoreDisplay;
	int m_HighScore{};
	bool m_HighScoreBroken{};
};

#endif // HIGHSCOREDISPLAY_H