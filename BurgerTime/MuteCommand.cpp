#include "MuteCommand.h"

#include <Locator.h>

MuteCommand::MuteCommand(real::GameObject* object)
	: Command(object)
{
}

void MuteCommand::Execute()
{
	m_IsMuted = !m_IsMuted;
	real::Locator::GetAudioSystem().Mute(m_IsMuted);
}
