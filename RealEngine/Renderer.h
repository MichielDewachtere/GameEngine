#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

namespace real
{
	class Texture2D;

	enum class RenderFlip
	{
		none = 0,
		vertical  = 1,
		horizontal = 2
	};

	class Renderer
	{
	public:
		virtual ~Renderer() = default;

		virtual void Render() const = 0;
		virtual void Destroy() = 0;

		virtual void RenderTexture(const Texture2D& texture, float x, float y) const = 0;
		virtual void RenderTexture(const Texture2D& texture, float x, float y, float width, float height) const = 0;

		virtual void RenderSprite(const Texture2D& texture, glm::ivec4 src, glm::ivec4 dst, float angle = 0, glm::ivec2 center = { 0,0 }, RenderFlip flip = RenderFlip::none) = 0;

		/**
		 * \brief Render a rectangle to the screen
		 * \param rect x = xPos, y = yPos, z = width, w = height
		 * \param isFilled if the rectangle should be filled or not
		 * \param color color of the rectangle
		 */
		virtual void RenderRectangle(const glm::ivec4& rect, bool isFilled, glm::u8vec4 color) const = 0;
		virtual void RenderPoint(int x, int y, glm::u8vec4 color) const = 0;
		virtual void RenderPoint(const glm::ivec2 pos, glm::u8vec4 color) const { RenderPoint(pos.x, pos.y, color); }
		virtual void RenderLine(int x1, int y1, int x2, int y2, glm::u8vec4 color) const = 0;
		virtual void RenderLine(const glm::ivec2 begin, const glm::ivec2 end, glm::u8vec4 color) const { RenderLine(begin.x, begin.y, end.x, end.y, color); }
		virtual void RenderShape(const glm::ivec2* points, int count, glm::u8vec4 color) const = 0;
		virtual void RenderShape(const std::vector<glm::ivec2>& points, glm::u8vec4  color) const = 0;

		const glm::u8vec4& GetBackgroundColor() const { return m_BackgroundColor; }
		void SetBackgroundColor(const glm::u8vec4& color) { m_BackgroundColor = color; }

	protected:
		glm::u8vec4 m_BackgroundColor{};
	};
}

#endif // RENDERER_H