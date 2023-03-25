#include <SDL.h>
#include "InputManager.h"

#include "backends/imgui_impl_sdl2.h"

bool real::InputManager::ProcessInput()
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}

		//process event for ImGui
		ImGui_ImplSDL2_ProcessEvent(&e);
	}

	return true;
}