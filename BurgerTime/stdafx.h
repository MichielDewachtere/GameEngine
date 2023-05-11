#ifndef STDAFX_H
#define STDAFX_H

#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS

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
#define NOMINMAX
#include <windows.h>

// STL Headers:
#include <string>
#include <memory>
#include <stdexcept>
#include <utility>
#include <functional>
#include <chrono>

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
#pragma comment(lib, "SDL2.dll")
#include <SDL_ttf.h>
#pragma comment(lib, "SDL2_ttf.dll")
#include <SDL_image.h>
#pragma comment(lib, "SDL2_image.dll")
#include <SDL_mixer.h>
#pragma comment(lib, "SDL2_mixer.dll")
#pragma endregion

#pragma region ImGui Headers
//#define USE_IMGUI

#ifdef USE_IMGUI
#include <imgui.h>
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl2.h"
#endif // USE_IMGUI
#pragma endregion

#pragma region Steam Headers
//#define USE_STEAM

#ifdef USE_STEAM
#include <steam_api.h>
#endif // USE_STEAM
#pragma endregion

#pragma endregion

#endif // STDAFX_H