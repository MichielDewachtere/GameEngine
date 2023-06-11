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
	HighScoreDisplay(const Component& other) = delete;
	HighScoreDisplay operator=(const Component& rhs) = delete;
	HighScoreDisplay(Component&& other) = delete;
	HighScoreDisplay operator=(Component&& rhs) = delete;
	
	void Update() override {}
	bool CanRender() const override { return false; }

	void HandleEvent(int) override;
	void OnSubjectDestroy() override {}
private:
	ScoreDisplay* m_ScoreDisplay;
	int m_HighScore{};
};

#endif // HIGHSCOREDISPLAY_H