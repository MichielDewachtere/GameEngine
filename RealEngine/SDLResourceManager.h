#ifndef SDLRESOURCEMANAGER_H
#define SDLRESOURCEMANAGER_H

#include <memory>
#include <string>

//#include "Singleton.h"
#include "ResourceManager.h"

namespace real
{
	//class SDLTexture2D;
	//class SDLFont;
	//class SDLResourceManager final : public Singleton<SDLResourceManager>
	//{
	//public:
	//	void Init(const std::string& data);
	//	std::shared_ptr<SDLTexture2D> LoadTexture(const std::string& file) const;
	//	std::shared_ptr<SDLFont> LoadFont(const std::string& file, unsigned int size) const;
	//private:
	//	friend class Singleton<SDLResourceManager>;
	//	SDLResourceManager() = default;
	//	std::string m_dataPath;
	//};

	class Font;
	class Texture2D;

	class SDLResourceManager final : public ResourceManager
	{
	public:
		explicit SDLResourceManager(std::string dataPath);
		virtual ~SDLResourceManager() override = default;

		SDLResourceManager(const SDLResourceManager& other) = delete;
		SDLResourceManager operator=(const SDLResourceManager& rhs) = delete;
		SDLResourceManager(SDLResourceManager&& other) = delete;
		SDLResourceManager operator=(SDLResourceManager&& rhs) = delete;

		virtual std::shared_ptr<Texture2D> LoadTexture(const std::string& file) const override;
		virtual std::shared_ptr<Font> LoadFont(const std::string& file, unsigned size) const override;

	private:
		std::string m_DataPath;
	};
}

#endif // SDLRESOURCEMANAGER_H
