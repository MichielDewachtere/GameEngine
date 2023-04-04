#ifndef STDAFX_H
#define STDAFX_H

#pragma region Third-Party Includes/Libs
#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif


#pragma region Windows Headers
// Windows Header Files:
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>

// STL Headers:
#include <string>
#include <memory>
#include <stdexcept>
#include <utility>
#include <functional>

// Containers:
#include <vector>
#include <map>

// Streams:
#include <iostream>
#pragma endregion

#pragma region GLM Headers
#include <glm/glm.hpp>
#pragma endregion

#pragma region SDL Headers
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#pragma endregion

#pragma region DirectX Headers
// XInput:
#include <Xinput.h>
#pragma endregion

#pragma region ImGui Headers
#include <imgui.h>
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl2.h"
#pragma endregion

#pragma region Steam Headers
//#define USE_STEAM

#ifdef USE_STEAM
#include <steam_api.h>
#endif // USE_STEAM
#pragma endregion

#pragma endregion

#endif // STDAFX_H