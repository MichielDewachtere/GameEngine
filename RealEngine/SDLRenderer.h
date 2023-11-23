#ifndef SDLRENDERER_H
#define SDLRENDERER_H

#include <SDL_render.h>
#include <SDL_video.h>

#include "Singleton.h"

namespace real
{
	class Texture2D;
	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class SDLRenderer final : public Singleton<SDLRenderer>
	{
		SDL_Renderer* m_renderer{};
		SDL_Window* m_window{};
		SDL_Color m_clearColor{};

		//ImGui
		bool m_ShowDemo{ false };
	public:
		void Init(SDL_Window* window, SDL_Color background);
		void Render();
		void Destroy();

		void RenderTexture(const Texture2D& texture, float x, float y) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float width, float height) const;

		SDL_Renderer* GetSDLRenderer() const;

		const SDL_Color& GetBackgroundColor() const { return m_clearColor; }
		void SetBackgroundColor(const SDL_Color& color) { m_clearColor = color; }
	};
}

#endif // SDLRENDERER_H