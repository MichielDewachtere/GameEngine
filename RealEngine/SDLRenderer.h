#ifndef SDLRENDERER_H
#define SDLRENDERER_H

#include <SDL_render.h>
#include <SDL_video.h>
#include <vector>
#include <glm/vec2.hpp>

#include "Renderer.h"

namespace real
{
	class SDLTexture2D;

	class SDLRenderer final : public Renderer
	{
	public:
		SDLRenderer(SDL_Window* window, glm::u8vec4 background);
		virtual ~SDLRenderer() override = default;

		SDLRenderer(const SDLRenderer& other) = delete;
		SDLRenderer operator=(const SDLRenderer& rhs) = delete;
		SDLRenderer(SDLRenderer&& other) = delete;
		SDLRenderer operator=(SDLRenderer&& rhs) = delete;

		void Init(SDL_Window* window, SDL_Color background);
		//void Render();
		virtual void Render() const override;
		virtual void Destroy() override;

		virtual void RenderTexture(const Texture2D& texture, float x, float y) const override;
		virtual void RenderTexture(const Texture2D& texture, float x, float y, float width, float height) const override;

		void RenderSprite(const Texture2D& texture, glm::ivec4 src, glm::ivec4 dst, float angle = 0, glm::ivec2 center = { 0,0 },
		                  RenderFlip flip = RenderFlip::none) override;

		virtual void RenderRectangle(const glm::ivec4& rect, bool isFilled, glm::u8vec4 color) const override;
		virtual void RenderPoint(int x, int y, glm::u8vec4 color) const override;
		virtual void RenderLine(int x1, int y1, int x2, int y2, glm::u8vec4 color) const override;
		virtual void RenderShape(const glm::ivec2* points, int count, glm::u8vec4 color) const override;
		virtual void RenderShape(const std::vector<glm::ivec2>& points, glm::u8vec4 color) const override;

		SDL_Renderer* GetSDLRenderer() const { return m_pRenderer; }

	private:
		SDL_Renderer* m_pRenderer{ nullptr };
		SDL_Window* m_pWindow{ nullptr };

		//ImGui
		bool m_ShowDemo{ false };

		static SDL_Point glmToSDLPoint(const glm::ivec2& point);
		static SDL_Rect glmToSDLRect(const glm::ivec4& rect);
		static SDL_Color glmToSDLColor(const glm::u8vec4& color);
	};
}

#endif // SDLRENDERER_H