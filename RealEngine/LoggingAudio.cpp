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

void real::LoggingAudio::Stop(const int channel)
{
	m_RealAudio->Stop(channel);
	Logger::LogInfo("Stopped playing channel {}", channel);
}

void real::LoggingAudio::Stop(const Sound sound)
{
	m_RealAudio->Stop(sound);
	Logger::LogInfo("Stopped playing sound {}, attached to file {}", sound.id, sound.fileName);
}

void real::LoggingAudio::StopAllSounds()
{
	m_RealAudio->StopAllSounds();
	Logger::LogInfo("Stopped all sounds from playing");
}

void real::LoggingAudio::Mute(bool mute)
{
	m_RealAudio->Mute(mute);
	if (mute)
		Logger::LogInfo("Muted all audio currently playing");
	else
		Logger::LogInfo("Un-muted all audio currently playing");

}
