#ifndef SDLRENDERER_H
#define SDLRENDERER_H

#include <SDL_render.h>
#include <SDL_video.h>
#include <vector>
#include <glm/vec2.hpp>

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

		void RenderRectangle(const SDL_Rect& rect, bool isFilled, SDL_Color color) const;
		void RenderPoint(int x, int y, SDL_Color color) const;
		void RenderPoint(const glm::ivec2 pos, SDL_Color color) const { RenderPoint(pos.x, pos.y, color); }
		void RenderLine(int x1, int y1, int x2, int y2, SDL_Color color) const;
		void RenderLine(const glm::ivec2 begin, const glm::ivec2 end, SDL_Color color) const { RenderLine(begin.x, begin.y, end.x, end.y, color); }
		void RenderShape(const SDL_Point* points, int count, SDL_Color color) const;
		void RenderShape(const std::vector<SDL_Point>& points, SDL_Color color) const;

		SDL_Renderer* GetSDLRenderer() const;

		const SDL_Color& GetBackgroundColor() const { return m_clearColor; }
		void SetBackgroundColor(const SDL_Color& color) { m_clearColor = color; }
	};
}

#endif // SDLRENDERER_H