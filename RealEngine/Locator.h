#ifndef LOCATOR_H
#define LOCATOR_H

#include "Audio.h"

namespace real
{
    class Locator final
    {
        inline static std::unique_ptr<Audio> m_AudioSystem = nullptr;
    public:
        static Audio& GetAudioSystem() { return *m_AudioSystem; }
        static void RegisterAudioSystem(Audio* audio) { m_AudioSystem.reset(audio); }
    };
}
#endif // LOCATOR_H