#include "PlayerNameDisplay.h"

#include <TextComponent.h>
#include <Input.h>

#include "PlayerManager.h"

PlayerNameDisplay::PlayerNameDisplay(real::GameObject* pOwner)
	: Component(pOwner)
{
}

PlayerNameDisplay::~PlayerNameDisplay()
{
	PlayerManager::GetInstance().SubmitName(m_PlayerName);

	real::Input::GetInstance().StopReadingKeyboard();
	real::Input::GetInstance().inputReceived.RemoveObserver(this);
}

void PlayerNameDisplay::Start()
{
	real::Input::GetInstance().StartReadingKeyboard();
	real::Input::GetInstance().inputReceived.AddObserver(this);
}

void PlayerNameDisplay::HandleEvent(char character)
{
	if (m_PlayerName.length() > 5)
		return;

	m_PlayerName = GetOwner()->GetComponent<real::TextComponent>()->GetText();
	m_PlayerName += character;

	GetOwner()->GetComponent<real::TextComponent>()->SetText(m_PlayerName);
}
