#ifndef SDLFONT_H
#define SDLFONT_H

#include <string>

#include "Font.h"

struct _TTF_Font;

namespace real
{
	class SDLFont final : public Font
	{
	public:
		explicit SDLFont(const std::string& fullPath, unsigned int size);
		virtual ~SDLFont() override;

		SDLFont(const SDLFont &) = delete;
		SDLFont(SDLFont &&) = delete;
		SDLFont & operator= (const SDLFont &) = delete;
		SDLFont & operator= (const SDLFont &&) = delete;

		_TTF_Font* GetFont() const;

		std::shared_ptr<Texture2D> CreateTexture(const std::string& text, glm::u8vec4 color) override;

	private:
		_TTF_Font* m_pFont;
		//unsigned int m_Size;
	};
}

#endif // SDLFONT_H