#ifndef SDLAUDIO_H
#define SDLAUDIO_H

#include "Audio.h"

namespace real
{
	class SDLAudio final : public Audio
	{
	public:
		explicit SDLAudio();
		virtual ~SDLAudio() override = default;
		SDLAudio(const SDLAudio& other) = delete;
		SDLAudio operator=(const SDLAudio& rhs) = delete;
		SDLAudio(SDLAudio&& other) = delete;
		SDLAudio operator=(SDLAudio&& rhs) = delete;

		void Update() override;

		void Play(const Sound sound, const int volume = -1, const int loops = -1) override;
		void Stop(const sound_id id) override;

		void AddSound(const Sound sound) override;

	private:
		bool IsLoaded(const Sound sound) override;
		void Load(const Sound sound) override;

	private:
		class SDLAudioImpl;
		std::unique_ptr<SDLAudioImpl> m_pImpl;
	};
}

#endif // SDLAUDIO_H