#ifndef LOGGINGRENDERER_H
#define LOGGINGRENDERER_H

#include <memory>

#include "Renderer.h"

namespace real
{
	class LoggingRenderer final : public Renderer
	{
	public:
		explicit LoggingRenderer(Renderer* pRenderer) : m_RealRenderer(pRenderer) {}
		virtual ~LoggingRenderer() override = default;

		LoggingRenderer(const LoggingRenderer& other) = delete;
		LoggingRenderer& operator=(const LoggingRenderer& rhs) = delete;
		LoggingRenderer(LoggingRenderer&& other) = delete;
		LoggingRenderer& operator=(LoggingRenderer&& rhs) = delete;

		void Render() const override;
		void Destroy() override;

		void RenderTexture(const Texture2D& texture, float x, float y) const override;
		void RenderTexture(const Texture2D& texture, float x, float y, float width, float height) const override;

		void RenderSprite(const Texture2D& texture, glm::ivec4 src, glm::ivec4 dst, float angle = 0, glm::ivec2 center = { 0,0 },
		                  RenderFlip flip = RenderFlip::none) override;

		void RenderRectangle(const glm::ivec4& rect, bool isFilled, glm::u8vec4 color) const override;
		void RenderPoint(int x, int y, glm::u8vec4 color) const override;
		void RenderLine(int x1, int y1, int x2, int y2, glm::u8vec4 color) const override;
		void RenderShape(const glm::ivec2* points, int count, glm::u8vec4 color) const override;
		void RenderShape(const std::vector<glm::ivec2>& points, glm::u8vec4 color) const override;

		Renderer& GetRenderer()const { return *m_RealRenderer; }

	private:
		std::unique_ptr<Renderer> m_RealRenderer;
	};
}

#endif // LOGGINGRENDERER_H