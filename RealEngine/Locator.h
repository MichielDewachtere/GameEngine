#ifndef LOCATOR_H
#define LOCATOR_H

#include <memory>

#include "Audio.h"
#include "NullAudio.h"

#include "Renderer.h"
#include "NullRenderer.h"

#include "ResourceManager.h"
#include "NullResourceManager.h"

namespace real
{
	class Locator final
    {
    public:
        static Audio& GetAudioSystem() { return *m_AudioSystem; }
        static void RegisterAudioSystem(Audio* audio);

        static Renderer& GetRenderSystem() { return *m_RenderSystem; }
        static void RegisterRenderSystem(Renderer* renderer);

        static ResourceManager& GetResourceSystem() { return *m_ResourceSystem; }
        static void RegisterResourceSystem(ResourceManager* resourceManager);

		static void ShutDown();
    private:
        inline static std::unique_ptr<Audio> m_AudioSystem = nullptr;
        inline static NullAudio m_DefaultAudioSystem{};

        inline static std::unique_ptr<Renderer> m_RenderSystem = nullptr;
        inline static NullRenderer m_DefaultRenderSystem{};

        inline static std::unique_ptr<ResourceManager> m_ResourceSystem = nullptr;
        inline static NullResourceManager m_DefaultResourceSystem{};
    };
}
#endif // LOCATOR_H