#include "HighScoreDisplay.h"

#include <TextComponent.h>
#include "HighScoreParser.h"

HighScoreDisplay::HighScoreDisplay(real::GameObject* pOwner, ScoreDisplay* pScoreDisplay)
	: Component(pOwner)
	, m_ScoreDisplay(pScoreDisplay)
{
	m_HighScore = HighScoreParser::ParseHighScores("highscores.json");
	GetOwner()->GetComponent<real::TextComponent>()->SetText(std::to_string(m_HighScore));

	m_ScoreDisplay->scoreChanged.AddObserver(this);
}

HighScoreDisplay::~HighScoreDisplay()
{
	m_ScoreDisplay->scoreChanged.RemoveObserver(this);
}

void HighScoreDisplay::HandleEvent(int newScore)
{
	if (newScore > m_HighScore)
	{
		m_HighScore = newScore;
		GetOwner()->GetComponent<real::TextComponent>()->SetText(std::to_string(m_HighScore));
	}
}
