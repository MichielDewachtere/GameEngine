#ifndef FONT_H
#define FONT_H

#include <memory>
#include <string>
#include <glm/vec4.hpp>

#include "Texture2D.h"

namespace real
{
	class Font
	{
	public:
		virtual ~Font() = default;

		virtual std::shared_ptr<Texture2D> CreateTexture(const std::string& text, glm::u8vec4 color) = 0;
	};
}

#endif // FONT_H