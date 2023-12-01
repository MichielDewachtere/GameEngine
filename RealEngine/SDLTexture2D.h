#ifndef SDLTEXTURE2D_H
#define SDLTEXTURE2D_H

#include <glm/vec2.hpp>

#include "Texture2D.h"

struct SDL_Texture;

namespace real
{
	/**
	 * Simple RAII wrapper for an SDL_Texture
	 */
	class SDLTexture2D final : public Texture2D
	{
	public:
		explicit SDLTexture2D(SDL_Texture* texture);
		virtual ~SDLTexture2D() override;
		SDLTexture2D(const SDLTexture2D&) = delete;
		SDLTexture2D & operator= (const SDLTexture2D&) = delete;
		SDLTexture2D(SDLTexture2D&&) = delete;
		SDLTexture2D & operator= (const SDLTexture2D&&) = delete;

		SDL_Texture* GetSDLTexture() const;

		virtual glm::ivec2 GetSize() const override;

	private:
		SDL_Texture* m_pTexture;
	};
}

#endif // SDLTEXTURE2D_H