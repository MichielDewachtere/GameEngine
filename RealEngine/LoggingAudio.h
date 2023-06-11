#ifndef LOGGINGAUDIO_H
#define LOGGINGAUDIO_H

#include <memory>

#include "Audio.h"

namespace real
{
	class LoggingAudio final : public Audio
	{
	public:
		explicit LoggingAudio(Audio* audio) : m_RealAudio(audio) {}
		virtual ~LoggingAudio() override = default;
		LoggingAudio(const LoggingAudio& other) = delete;
		LoggingAudio operator=(const LoggingAudio& rhs) = delete;
		LoggingAudio(LoggingAudio&& other) = delete;
		LoggingAudio operator=(LoggingAudio&& rhs) = delete;

		virtual void Update() override;
		virtual void Play(const Sound sound, const int volume, const int loops) override;
		virtual void Stop(const int channel) override;
		virtual void Stop(const Sound sound) override;
		virtual void StopAllSounds() override;
		virtual void Mute(bool mute) override;
	private:
		std::unique_ptr<Audio> m_RealAudio;
	};
}

#endif // LOGGINGAUDIO_H