//#include "stdafx.h"
#include "SDLResourceManager.h"

#include <codecvt>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdexcept>

#include "Locator.h"
#include "LoggingRenderer.h"
#include "SDLRenderer.h"
#include "SDLTexture2D.h"
#include "SDLFont.h"

real::SDLResourceManager::SDLResourceManager(std::string dataPath)
	: ResourceManager(dataPath)
{
	if (TTF_Init() != 0)
	{
		throw std::runtime_error(std::string("Failed to load support for fonts: ") + SDL_GetError());
	}
}

std::shared_ptr<real::Texture2D> real::SDLResourceManager::LoadTexture(const std::string& file) const
{
	SDL_Renderer* renderer;
	const auto fullPath = m_DataPath + file;

#ifdef _DEBUG
	renderer = dynamic_cast<SDLRenderer*>(&dynamic_cast<LoggingRenderer*>(&Locator::GetRenderSystem())->GetRenderer())->GetSDLRenderer();
#else
	renderer = dynamic_cast<SDLRenderer*>(&Locator::GetRenderSystem())->GetSDLRenderer();
#endif

	auto texture = IMG_LoadTexture(renderer, fullPath.c_str());
	if (texture == nullptr)
	{
		throw std::runtime_error(std::string("Failed to load texture: ") + SDL_GetError());
	}

	return std::make_shared<SDLTexture2D>(texture);
}

std::shared_ptr<real::Font> real::SDLResourceManager::LoadFont(const std::string& file, unsigned size) const
{
	const auto fullPath = m_DataPath + file;
	return std::make_shared<SDLFont>(fullPath.c_str(), size);
}