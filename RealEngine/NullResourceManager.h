#ifndef NULLRESOURCEMANAGER_H
#define NULLRESOURCEMANAGER_H

#include "ResourceManager.h"

namespace real
{
	class NullResourceManager final : public ResourceManager
	{
	public:
		explicit NullResourceManager(std::string dataPath = "") : ResourceManager(std::move(dataPath)) {}
		virtual ~NullResourceManager() override = default;

		NullResourceManager(const NullResourceManager& other) = delete;
		NullResourceManager& operator=(const NullResourceManager& rhs) = delete;
		NullResourceManager(NullResourceManager&& other) = delete;
		NullResourceManager& operator=(NullResourceManager&& rhs) = delete;

		std::shared_ptr<Texture2D> LoadTexture(const std::string&) const override { return nullptr; }
		std::shared_ptr<Font> LoadFont(const std::string&, unsigned) const override { return nullptr; }

	private:

	};
}

#endif // NULLRESOURCEMANAGER_H