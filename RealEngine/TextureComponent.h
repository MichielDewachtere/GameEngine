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

		void Scale(const float uniformScale);
		void Scale(const float scaleX, const float scaleY);

		bool CanRender() const override { return true; }

	private:
		std::shared_ptr<Texture2D> m_pTexture{ nullptr };
		glm::vec2 m_Scale{1, 1};
	};
}

#endif // TEXTURECOMPONENT_H