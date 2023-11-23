#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include <glm/vec2.hpp>

struct SDL_Texture;

namespace real
{
	/**
	 * Simple RAII wrapper for an SDL_Texture
	 */
	class Texture2D final
	{
	public:
		explicit Texture2D(SDL_Texture* texture);
		~Texture2D();
		Texture2D(const Texture2D &) = delete;
		Texture2D(Texture2D &&) = delete;
		Texture2D & operator= (const Texture2D &) = delete;
		Texture2D & operator= (const Texture2D &&) = delete;

		SDL_Texture* GetSDLTexture() const;

		glm::ivec2 GetSize() const;

		void SetScale(const float uniformScale);
		void SetScale(const float scaleX, const float scaleY);
		glm::vec2 GetScale() const { return m_Scale; }

	private:
		SDL_Texture* m_pTexture;
		glm::vec2 m_Scale{1, 1};
	};
}

#endif // TEXTURE2D_H