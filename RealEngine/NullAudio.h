#ifndef NULLAUDIO_H
#define NULLAUDIO_H

#include "Audio.h"

namespace real
{
	class NullAudio final : public Audio
	{
	public:
		NullAudio() : Audio() {}
		virtual ~NullAudio() override = default;

		virtual void Update() override {}
		virtual void Play(const Sound, const int, const int) override {}
		virtual void Stop(const Sound) override {}
	};
}
#endif // NULLAUDIO_H