#ifndef TEXTURECOMPONENT_H
#define TEXTURECOMPONENT_H

#include "stdafx.h"

#include "Component.h"
#include "Texture2D.h"

namespace real
{
	class TextureComponent final : public Component
	{
	public:
		explicit TextureComponent(GameObject* pOwner) : Component(pOwner) {}
		~TextureComponent() override = default;
		TextureComponent(const TextureComponent& other) = delete;
		TextureComponent& operator=(const TextureComponent& rhs) = delete;
		TextureComponent(TextureComponent&& other) = delete;
		TextureComponent& operator=(TextureComponent&& rhs) = delete;

		void Update() override {};
		void Render() const override;

		void SetTexture(std::shared_ptr<Texture2D> pTexture) { m_pTexture = std::move(pTexture); }

		bool CanRender() const override { return true; }

	private:
		std::shared_ptr<Texture2D> m_pTexture{ nullptr };
	};
}

#endif // TEXTURECOMPONENT_H