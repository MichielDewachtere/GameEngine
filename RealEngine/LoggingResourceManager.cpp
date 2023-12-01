#include "LoggingResourceManager.h"

#include <codecvt>

#include "Logger.h"

std::shared_ptr<real::Texture2D> real::LoggingResourceManager::LoadTexture(const std::string& file) const
{
	Logger::LogInfo({"Loaded texture from file: {}"}, file);
	return m_RealResourceManager->LoadTexture(file);
}

std::shared_ptr<real::Font> real::LoggingResourceManager::LoadFont(const std::string& file, unsigned size) const
{
	Logger::LogInfo({"Loaded font from file: {}, with size {}"}, file, size);
	return m_RealResourceManager->LoadFont(file, size);
}
