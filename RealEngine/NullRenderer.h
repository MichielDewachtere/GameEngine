#ifndef NULLRENDERER_H
#define NULLRENDERER_H

#include "Renderer.h"

namespace real
{
	class NullRenderer final : public Renderer
	{
	public:
		explicit NullRenderer() = default;
		virtual ~NullRenderer() override = default;

		NullRenderer(const NullRenderer& other) = delete;
		NullRenderer& operator=(const NullRenderer& rhs) = delete;
		NullRenderer(NullRenderer&& other) = delete;
		NullRenderer& operator=(NullRenderer&& rhs) = delete;

		void Render() const override {}
		void Destroy() override {}
		void RenderTexture(const Texture2D&, float, float) const override {}
		void RenderTexture(const Texture2D&, float, float, float, float) const override {}
		void RenderSprite(const Texture2D&, glm::ivec4, glm::ivec4, float, glm::ivec2,
		                  RenderFlip) override {}
		void RenderRectangle(const glm::ivec4&, bool, glm::u8vec4) const override {}
		void RenderPoint(int, int, glm::u8vec4) const override {}
		void RenderLine(int, int, int, int, glm::u8vec4) const override {}
		void RenderShape(const glm::ivec2*, int, glm::u8vec4) const override {}
		void RenderShape(const std::vector<glm::ivec2>&, glm::u8vec4) const override {}

	private:

	};
}

#endif // NULLRENDERER_H