#include "QuitCommand.h"

#include <cstdlib>

QuitCommand::QuitCommand(real::GameObject* object)
	: Command(object)
{
}

void QuitCommand::Execute()
{
	std::exit(0);
}
