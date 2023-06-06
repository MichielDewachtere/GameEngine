#include "StunCommand.h"

#include "GameObject.h"
#include "PlayerCharacter.h"

StunCommand::StunCommand(real::GameObject* object)
	: Command(object)
{
}

void StunCommand::Execute()
{
	// create temporarely 
	// notify enemies
	GetOwner()->GetComponent<PlayerCharacter>()->ThrowPepper();
}
