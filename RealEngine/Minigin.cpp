//#include "stdafx.h"
#include "Minigin.h"

#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <SDL_version.h>

#include "Input.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "SDLRenderer.h"
#include "SDLResourceManager.h"
#include "GameTime.h"
#include "Locator.h"
#include "LoggingAudio.h"
#include "LoggingRenderer.h"
#include "LoggingResourceManager.h"
#include "SDLAudio.h"
#include "Logger.h"

SDL_Window* g_window{};

void PrintSDLVersion()
{
	SDL_version version;
	SDL_VERSION(&version)
	printf("We compiled against SDL version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	SDL_GetVersion(&version);
	printf("We are linking against SDL version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_IMAGE_VERSION(&version)
	printf("We compiled against SDL_image version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *IMG_Linked_Version();
	printf("We are linking against SDL_image version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_TTF_VERSION(&version)
	printf("We compiled against SDL_ttf version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *TTF_Linked_Version();
	printf("We are linking against SDL_ttf version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_MIXER_VERSION(&version)
	printf("We compiled against SDL_mixer version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *Mix_Linked_Version();
	printf("We are linking against SDL_mixer version %u.%u.%u.\n",
		version.major, version.minor, version.patch);
}

real::Minigin::Minigin(const std::string &dataPath, const WindowSettings& window)
{
	PrintSDLVersion();
	
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	g_window = SDL_CreateWindow(
		window.title.c_str(),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		window.width,
		window.height,
		SDL_WINDOW_OPENGL
	);
	if (g_window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Time::GetInstance().Init();
	Logger::Initialize();

#if _DEBUG
	Locator::RegisterAudioSystem(new LoggingAudio(new SDLAudio()));
	Locator::RegisterRenderSystem(new LoggingRenderer(new SDLRenderer(g_window, window.background)));
	Locator::RegisterResourceSystem(new LoggingResourceManager(new SDLResourceManager(dataPath)));
#else
	Locator::RegisterAudioSystem(new SDLAudio());
	Locator::RegisterRenderSystem(new SDLRenderer(g_window, window.background));
	Locator::RegisterResourceSystem(new SDLResourceManager(dataPath));
#endif
}

real::Minigin::~Minigin()
{
	Locator::GetRenderSystem().Destroy();
	SDL_DestroyWindow(g_window);
	g_window = nullptr;
	SDL_Quit();
}

void real::Minigin::Run(const std::function<void()>& load)
{
	load();

	const auto& renderer = Locator::GetRenderSystem();
	auto& sceneManager = SceneManager::GetInstance();
	auto& input = Input::GetInstance();
	auto& time = Time::GetInstance();
	auto& audio = Locator::GetAudioSystem();

	// todo: this update loop could use some work.
	bool doContinue = true;
	auto lastTime = std::chrono::high_resolution_clock::now();
	constexpr int desiredFps = 144;	
	constexpr int desiredFrameTime = 1000 / desiredFps;
	float lag = 0.0f;
	while (doContinue)
	{
		constexpr float fixedTimeStep = 0.02f;
		const auto currentTime = std::chrono::high_resolution_clock::now();
		const float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
		lastTime = currentTime;
		lag += deltaTime;

		while (lag >= fixedTimeStep)
		{
			// Fixed Update
			lag -= fixedTimeStep;
		}

		doContinue = input.ProcessInput();

		input.Update();

#ifdef USE_STEAM
		SteamAPI_RunCallbacks();
#endif // USE_STEAM
		time.Update();
		sceneManager.Update();
		renderer.Render();
		audio.Update();

		sceneManager.PostUpdate();

		const auto sleepTime = currentTime + std::chrono::milliseconds(desiredFrameTime) - std::chrono::high_resolution_clock::now();
		std::this_thread::sleep_for(sleepTime);
	}
}