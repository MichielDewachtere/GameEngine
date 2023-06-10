#include "PepperCounter.h"

#include <TextComponent.h>

#include "PlayerCharacter.h"
#include "PlayerManager.h"

PepperCounter::PepperCounter(real::GameObject* pOwner)
	: Component(pOwner)
{
}

PepperCounter::~PepperCounter()
{
	for (const auto& pPlayer : PlayerManager::GetInstance().GetPlayers())
	{
		pPlayer->GetComponent<PlayerCharacter>()->amountOfPepperChanged.RemoveObserver(this);
	}
}

void PepperCounter::Start()
{
 	for (const auto& pPlayer : PlayerManager::GetInstance().GetPlayers())
	{
		pPlayer->GetComponent<PlayerCharacter>()->amountOfPepperChanged.AddObserver(this);
	}

	UpdatePepperText(5);
}

void PepperCounter::HandleEvent(int pepperLeft)
{
	UpdatePepperText(pepperLeft);
}

void PepperCounter::UpdatePepperText(int pepperLeft)
{
   	GetOwner()->GetComponent<real::TextComponent>()->SetText(std::to_string(pepperLeft));
}
