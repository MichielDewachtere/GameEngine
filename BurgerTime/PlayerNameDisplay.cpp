#include "PlayerNameDisplay.h"

#include <TextComponent.h>
#include <Input.h>

PlayerNameDisplay::PlayerNameDisplay(real::GameObject* pOwner)
	: Component(pOwner)
{
}

PlayerNameDisplay::~PlayerNameDisplay()
{
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
	auto text = GetOwner()->GetComponent<real::TextComponent>()->GetText();
	text += character;

	GetOwner()->GetComponent<real::TextComponent>()->SetText(text);
}
