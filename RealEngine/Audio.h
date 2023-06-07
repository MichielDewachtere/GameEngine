#ifndef AUDIO_H
#define AUDIO_H

#include <array>
#include <string>

namespace real
{
	using sound_id = unsigned short;

	struct Sound
	{
		sound_id id;
		int volume;
		int loops;
		std::string fileName;

		bool operator<(const Sound& other) const
		{
			return id < other.id;
		}
	};

	class Audio
	{
	public:
		virtual ~Audio() = default;

		virtual void Update() = 0;

		/**
		 * \brief 
		 * \param volume default = -1 : default value defined in struct is used, otherwise the value defined in the struct will be overriden;
		 * \param loops default = -1 : default value defined in struct is used, otherwise the value defined in the struct will be overriden;
		 */
		virtual void Play(const Sound sound, const int volume = -1, const int loops = -1) = 0;
		virtual void Stop(const Sound sound) = 0;
		virtual void StopAllSounds() = 0;
		virtual void Mute(bool mute) = 0;
	protected:
	};
}

#endif // AUDIO_H