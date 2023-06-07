#include "PlayerJoinCommand.h"

#include "PlayerManager.h"
#include "XInputController.h"

PlayerJoinCommand::PlayerJoinCommand(real::GameObject* object)
	: Command(object)
{
}

void PlayerJoinCommand::Execute()
{
	if (GetController() == nullptr)
		PlayerManager::GetInstance().AddPlayer();
	else
		PlayerManager::GetInstance().AddPlayer(false, GetController()->GetIndex());
}
