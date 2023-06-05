#ifndef LOGGINGAUDIO_H
#define LOGGINGAUDIO_H

#include <memory>

#include "Audio.h"

namespace real
{
	class LoggingAudio final : public Audio
	{
	public:
		LoggingAudio(Audio* audio) : m_RealAudio(audio) {}
		virtual ~LoggingAudio() override = default;

		void Update() override;
		void Play(const Sound sound, const int volume, const int loops) override;
		void Stop(const Sound sound) override;
	private:
		std::unique_ptr<Audio> m_RealAudio;
	};
}

#endif // LOGGINGAUDIO_H