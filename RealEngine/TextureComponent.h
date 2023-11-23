#ifndef TEXTURECOMPONENT_H
#define TEXTURECOMPONENT_H

#include "Component.h"
#include "Texture2D.h"

namespace real
{
	class TextureComponent final : public Component
	{
	public:
		explicit TextureComponent(GameObject* pOwner) : Component(pOwner) {}
		virtual ~TextureComponent() override = default;
		TextureComponent(const TextureComponent& other) = delete;
		TextureComponent& operator=(const TextureComponent& rhs) = delete;
		TextureComponent(TextureComponent&& other) = delete;
		TextureComponent& operator=(TextureComponent&& rhs) = delete;

		void Update() override {}
		void Render() const override;

		void SetTexture(std::shared_ptr<Texture2D> pTexture) { m_pTexture = std::move(pTexture); }
		Texture2D* GetTexture() const { return m_pTexture.get(); }

		void SetText(std::shared_ptr<Texture2D> pText) { m_pText = std::move(pText); }
		Texture2D* GetText() const { return m_pText.get(); }

		void SetRenderOffset(const glm::ivec2 offset) { m_RenderOffset = offset; }
		void SetRenderOffset(int x, int y) { SetRenderOffset({ x,y }); }
		glm::ivec2 GetRenderOffset() const { return m_RenderOffset; }

		void SetTextRenderOffset(const glm::ivec2 offset) { m_TextRenderOffset = offset; }
		void SetTextRenderOffset(int x, int y) { SetRenderOffset({ x,y }); }
		glm::ivec2 GetTextRenderOffset() const { return m_TextRenderOffset; }

		bool CanRender() const override { return true; }

	private:
		std::shared_ptr<Texture2D> m_pTexture{ nullptr }, m_pText{ nullptr };
		glm::ivec2 m_RenderOffset{0, 0}, m_TextRenderOffset{ 0,0 };
	};
}

#endif // TEXTURECOMPONENT_H