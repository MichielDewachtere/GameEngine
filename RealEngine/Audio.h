#ifndef AUDIO_H
#define AUDIO_H

#include <array>

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
		virtual void Stop(const sound_id id) = 0;

		virtual void AddSound(const Sound sound) = 0;

	protected:
		int m_Head, m_Tail;
		static constexpr size_t max_pending = 16;
		std::array<Sound, max_pending> m_Pending{};
		int m_NumPending{};

		virtual bool IsLoaded(const Sound sound) = 0;
		virtual void Load(const Sound sound) = 0;
	};
}

#endif // AUDIO_H