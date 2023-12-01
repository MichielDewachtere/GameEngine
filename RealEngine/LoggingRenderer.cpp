#include "LoggingRenderer.h"

#include "Logger.h"
#include "SDLTexture2D.h"

void real::LoggingRenderer::Render() const
{
	m_RealRenderer->Render();
}

void real::LoggingRenderer::Destroy()
{
	m_RealRenderer->Destroy();
	Logger::LogInfo({"Destroyed Renderer"});
}

void real::LoggingRenderer::RenderTexture(const Texture2D& texture, float x, float y) const
{
	m_RealRenderer->RenderTexture(texture, x, y);
	//Logger::LogInfo("Rendered texture {}, at position {}; {}", &texture, x, y);
}

void real::LoggingRenderer::RenderTexture(const Texture2D& texture, float x, float y, float width, float height) const
{
	m_RealRenderer->RenderTexture(texture, x, y, width, height);
	//Logger::LogInfo("Rendered texture {}, at position {}; {}, with width {} and height {}", &texture, x, y, width , height);
}

void real::LoggingRenderer::RenderSprite(const Texture2D& texture, glm::ivec4 src, glm::ivec4 dst, float angle, glm::ivec2 center, RenderFlip flip)
{
	m_RealRenderer->RenderSprite(texture, src, dst, angle, center, flip);
	//Logger::LogInfo("Rendered sprite {}, at position {}; {}, with width {} and height {}", &texture, dst.x, dst.y);
}

void real::LoggingRenderer::RenderRectangle(const glm::ivec4& rect, bool isFilled, glm::u8vec4 color) const
{
	m_RealRenderer->RenderRectangle(rect, isFilled, color);
	//Logger::LogInfo("Rendered a rectangle to the screen at position {}; {} and dimensions {}; {}", rect.x, rect.y, rect.z, rect.w);
}

void real::LoggingRenderer::RenderPoint(int x, int y, glm::u8vec4 color) const
{
	m_RealRenderer->RenderPoint(x, y, color);
	//Logger::LogInfo("Rendered a point to the screen at position {}; {}", x, y);
}

void real::LoggingRenderer::RenderLine(int x1, int y1, int x2, int y2, glm::u8vec4 color) const
{
	m_RealRenderer->RenderLine(x1, y1, x2, y2, color);
	//Logger::LogInfo("Rendered a line to the screen from {}; {}, to {}; {}", x1, y1, x2, y2);
}

void real::LoggingRenderer::RenderShape(const glm::ivec2* points, int count, glm::u8vec4 color) const
{
	m_RealRenderer->RenderShape(points, count, color);
	//Logger::LogInfo("Rendered a shape to the screen at position {}; {}, containing {} points", points[0].x, points[0].y, count);
}

void real::LoggingRenderer::RenderShape(const std::vector<glm::ivec2>& points, glm::u8vec4 color) const
{
	m_RealRenderer->RenderShape(points, color);
	//Logger::LogInfo("Rendered a shape to the screen at position {}; {}, containing {} points", points[0].x, points[0].y, points.size());
}