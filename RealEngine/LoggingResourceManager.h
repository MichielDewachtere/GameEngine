#ifndef LOGGINGRESOURCEMANAGER_H
#define LOGGINGRESOURCEMANAGER_H

#include "ResourceManager.h"

namespace real
{
	class LoggingResourceManager final : public ResourceManager
	{
	public:
		explicit LoggingResourceManager(ResourceManager* resourceManager) : ResourceManager(""), m_RealResourceManager(resourceManager) {}
		virtual ~LoggingResourceManager() override = default;

		LoggingResourceManager(const LoggingResourceManager& other) = delete;
		LoggingResourceManager& operator=(const LoggingResourceManager& rhs) = delete;
		LoggingResourceManager(LoggingResourceManager&& other) = delete;
		LoggingResourceManager& operator=(LoggingResourceManager&& rhs) = delete;

		std::shared_ptr<Texture2D> LoadTexture(const std::string& file) const override;
		std::shared_ptr<Font> LoadFont(const std::string& file, unsigned size) const override;
	private:
		std::unique_ptr<ResourceManager> m_RealResourceManager;
	};
}

#endif // LOGGINGRESOURCEMANAGER_H