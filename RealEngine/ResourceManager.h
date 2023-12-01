#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <string>
#include <memory>

namespace real
{
	class Texture2D;
	class Font;

	class ResourceManager
	{
	public:
		virtual ~ResourceManager() = default;

		virtual std::shared_ptr<Texture2D> LoadTexture(const std::string& file) const = 0;
		virtual std::shared_ptr<Font> LoadFont(const std::string& file, unsigned int size) const = 0;
	};
}

#endif // RESOURCEMANAGER_H