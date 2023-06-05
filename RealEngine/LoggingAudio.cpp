//#include "stdafx.h"
#include "LoggingAudio.h"

#include "Logger.h"

void real::LoggingAudio::Update()
{
	m_RealAudio->Update();
}

void real::LoggingAudio::Play(const Sound sound, const int volume, const int loops)
{
	m_RealAudio->Play(sound, volume, loops);
	Logger::LogInfo("Started playing sound {}, attached to file {}", sound.id, sound.fileName);
}

void real::LoggingAudio::Stop(const Sound sound)
{
	m_RealAudio->Stop(sound);
	Logger::LogInfo("Stopped playing sound {}, attached to file {}", sound.id, sound.fileName);
}
