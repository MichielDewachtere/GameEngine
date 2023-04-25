#include "stdafx.h"
#include "LivesDisplay.h"

#include "HealthComponent.h"
#include "TextComponent.h"

LivesDisplay::LivesDisplay(real::GameObject* pOwner, HealthComponent* pHealth)
	: Component(pOwner)
	, m_Health(pHealth)
{
	m_Health->healthChanged.AddObserver(this);
	UpdateLivesText();
}

LivesDisplay::~LivesDisplay()
{
	if (m_Health)
		m_Health->healthChanged.RemoveObserver(this);
}

void LivesDisplay::HandleEvent()
{
	UpdateLivesText();
}

void LivesDisplay::UpdateLivesText() const
{
	this->GetOwner()->GetComponent<real::TextComponent>()->SetText("Lives :" + std::to_string(m_Health->GetLives()));
}
