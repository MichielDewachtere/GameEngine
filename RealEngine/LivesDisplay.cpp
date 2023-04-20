#include "stdafx.h"
#include "LivesDisplay.h"

#include "HealthComponent.h"
#include "TextComponent.h"

real::LivesDisplay::LivesDisplay(GameObject* pOwner, HealthComponent* pHealth)
	: Component(pOwner)
	, m_Health(pHealth)
{
	m_Health->healthChanged.AddObserver(this);
	UpdateLivesText();
}

real::LivesDisplay::~LivesDisplay()
{
	if (m_Health)
		m_Health->healthChanged.RemoveObserver(this);
}

void real::LivesDisplay::HandleEvent()
{
	UpdateLivesText();
}

void real::LivesDisplay::UpdateLivesText() const
{
	this->GetOwner()->GetComponent<TextComponent>()->SetText("Lives :" + std::to_string(m_Health->GetLives()));
}
