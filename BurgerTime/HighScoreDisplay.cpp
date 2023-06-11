#include "HighScoreDisplay.h"

#include <TextComponent.h>
#include <Locator.h>

#include "HighScoreParser.h"
#include "GameInfo.h"

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
		if (m_HighScoreBroken == false)
		{
			real::Locator::GetAudioSystem().Play(Sounds::high_score);
			m_HighScoreBroken = true;
		}

		m_HighScore = newScore;
		GetOwner()->GetComponent<real::TextComponent>()->SetText(std::to_string(m_HighScore));
	}
}
